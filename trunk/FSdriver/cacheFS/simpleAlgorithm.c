/*
 * simpleAlgorithm.c
 *
 *  Created on: Dec 4, 2009
 *      Author: apophys
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "cachefs.h"
#include "cachingAlgorithms.h"
#include "simpleAlgorithm.h"


static char* fsRoot=NULL;
static char* ssdMountRoot=NULL;
static char* ramMountRoot=NULL;

static cacheControl* cc=NULL;

struct fileCacheRecord
{
	char* path;				// relative path of the file. It's the filepath provided by FUSE
	char* hddPath;			// absolute path of the file on the HDD

	off_t fileSize;			// file size

	long long accessCount;	// the number of times the file has been accessed
	time_t lastAccess;		// timestamp of the last access

	int isRAM;				// the file exists in RAM
	int isSSD;				// the file exists on the SSD
};


static struct fileCacheRecord* rec=NULL;
static int rec_n=0;
static int rec_step=100;
static int rec_size=100;


#define PFREE_RAM 15		/* Threshold percentage of free space for calling garbage collect on RAM */
#define PFREE_SSD 5			/* Threshold percentage of free space for calling garbage collect on SSD */


void recordAccess_simpleAlg(const char*  path, const char* hddPath );
int getAction_simpleAlg(const char* path );


int initCacheStructs_simpleAlg(void* parameters)
{
	struct params_simpleAlg* p= parameters;

	printf("initCacheStructs_simpleAlg \n");

	fsRoot = strdup(p->fsRoot);
	ssdMountRoot = strdup(p->ssdMountPoint);
	ramMountRoot = strdup(p->ramMountPoint);


	rec = (struct fileCacheRecord*) malloc( rec_size * sizeof(struct fileCacheRecord));

	return 0;
}

int releaseCacheStructs_simpleAlg()
{
	free(fsRoot);
	free(ssdMountRoot);
	free(ramMountRoot);

	free(rec);

	return 0;
}

int initCachingAlgorithm_simpleAlg(cachingAlgoritm* alg, cacheControl* ccontrol)
{
	memset(alg,0,sizeof(cachingAlgoritm));

	alg->initCacheStructs = initCacheStructs_simpleAlg;
	alg->releaseCacheStructs = releaseCacheStructs_simpleAlg;

	alg->recordAccess = recordAccess_simpleAlg;
	alg->getAction = getAction_simpleAlg;

	cc = ccontrol;

	return 0;
}




void recordAccess_simpleAlg(const char*  path, const char* hddPath )
{
	int i;
	struct stat buf;


	// search if the file exists
	for (i=0; i<rec_n; i++)
		if ( strcmp(rec[i].path, path) == 0 )
		{
			// file record found, update it

			rec[i].accessCount++;
			rec[i].lastAccess = time(NULL);

			return;
		}

	// file record does not exist, create one

	if ( rec_n >= rec_size )
	{
		// extend array

		rec_size+=rec_step;
		rec = (struct fileCacheRecord*) realloc(rec, rec_size * sizeof(struct fileCacheRecord));

	}


	rec[rec_n].path = strdup(path);
	rec[rec_n].hddPath = strdup(hddPath);

	rec[rec_n].accessCount = 1;
	rec[rec_n].lastAccess = time(NULL);

	rec[rec_n].isRAM = 0;
	rec[rec_n].isSSD = 0;


	stat(hddPath, &buf);
	rec[rec_n].fileSize = buf.st_size;	//TODO: replace with size in blocks*bsize, not just bytes

	rec_n++;

}

static struct fileCacheRecord* getFileRecord(const char* path )
{
	int i;

	for (i=0; i<rec_n; i++)
		if ( strcmp(rec[i].path, path) == 0 )
			return &rec[i];

	return NULL;
}


int getAction_simpleAlg(const char* path )
{
	struct fileCacheRecord* r;
	long long freeRAM, freeSSD;
	long long totalRAM, totalSSD;
	int pRAM, pSSD;

	r = getFileRecord(path);

	if (!r)
		return READ_FROM_HDD;				// ..? maybe error would be better

	if (r->isRAM)
		return READ_FROM_RAM;				// file is in RAM, read directly, no further analysis needed


	freeRAM = cc->getFreeSpace(LVL_RAM);
	freeSSD = cc->getFreeSpace(LVL_SSD);

	totalRAM = cc->getTotalSpace(LVL_RAM);
	totalSSD = cc->getTotalSpace(LVL_SSD);

	pRAM =  (freeRAM - r->fileSize )/(double)totalRAM  * 100.0;	// percentage of free RAM remaining if the file is copied there


	if (pRAM > PFREE_RAM)					// sufficient space remains for the file to be copied to RAM
	{
		r->isRAM = 1;

		if (r->isSSD)						// copy from SSD
			return COPY_SSD_RAM;

		return COPY_HDD_RAM;				// copy from HDD
	}


	pSSD =  (freeSSD - r->fileSize )/(double)totalSSD  * 100.0;	// percentage of free SSD remaining if the file is copied there

	if (pSSD > PFREE_SSD)					// sufficient space remains for the file to be copied to SSD
	{
		r->isSSD = 1;

		return COPY_HDD_SSD;				// copy from HDD

	}


	// insufficient space for caching the files. maybe call garbage collector?

	//cc->cleanup();


	return READ_FROM_HDD;
}


