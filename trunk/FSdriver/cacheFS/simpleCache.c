#ifdef linux
/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500
#endif

#include <stdlib.h>
#include <stdio.h>
#include <sys/statfs.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#include "cachefs.h"
#include "simpleCache.h"

#include "cachingAlgorithms.h"
#include "simpleAlgorithm.h"

char* ramMountPoint=NULL;
char* ssdMountPoint=NULL;
char* hddMountPoint=NULL;
char* fsRoot=NULL;

static cachingAlgoritm alg;


static int getattr_simpleCache(const char *path, struct stat *stbuf);
static int getdir_simpleCache(const char *path, fuse_dirh_t h, fuse_dirfil_t filler);
static int open_simpleCache(const char *path, struct fuse_file_info *fi);
static int release_simpleCache(const char *path, struct fuse_file_info *);
static int read_simpleCache(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);


struct fuse_operations oper=
{
	    .getattr = getattr_simpleCache,

	    .getdir	 = getdir_simpleCache, 	// TODO: deprecated, replace with readdir

	    .open	 = open_simpleCache,
	    .release = release_simpleCache,

	    .read	 = read_simpleCache,
};

long long getFreeSpace_simpleCache(int level)
{
	struct statfs buf;
	long long res;

	switch (level)
	{
		case LVL_RAM:	res = statfs(ramMountPoint, &buf); break;
		case LVL_SSD:	res = statfs(ssdMountPoint, &buf); break;
		default:		return -1;
	}

	if (res !=0)	// error
		return -1;

	res = buf.f_bsize * buf.f_bavail;

	return res;	// return free space

}

long long getTotalSpace_simpleCache(int level)
{
	struct statfs buf;
	long long res;

	switch (level)
	{
		case LVL_RAM:	res = statfs(ramMountPoint, &buf); break;
		case LVL_SSD:	res = statfs(ssdMountPoint, &buf); break;
		default:		return -1;
	}

	if (res !=0)	// error
		return -1;

	res = buf.f_bsize * buf.f_blocks;

	return res;	// return total space

}

int initCache_simpleCache(int* ramSize, int* ssdSize, void* parameters , struct fuse_operations **operations)
{
	long long freeR, freeS;
	struct params_simpleCache* params= parameters;
	struct params_simpleAlg pAlg;

	printf("initCache_simple\n");

	ssdMountPoint = strdup(params->ssdMountPoint);
	ramMountPoint = strdup(params->ramMountPoint);
	hddMountPoint = strdup(params->hddMountPoint);
	fsRoot = strdup(params->cachefsMountPoint);


	freeR = getFreeSpace_simpleCache(LVL_RAM);
	freeS = getFreeSpace_simpleCache(LVL_SSD);

	if (freeR < 0 || freeS < 0)	// error
		return -1;

	if (freeR < *ramSize || freeS < *ssdSize)	// insufficient space for allocation
		return -1;


	pAlg.fsRoot = params->cachefsMountPoint;
	pAlg.ssdMountPoint = params->ssdMountPoint;
	pAlg.ramMountPoint = params->ramMountPoint;

	alg.initCacheStructs(&pAlg);

	*operations = &oper;


	return 0;
}

int releaseCache_simpleCache()
{
	free(ssdMountPoint);
	free(ramMountPoint);
	free(hddMountPoint);
	free(fsRoot);

	return 0;
}

int cacheFile_simpleCache(const char* path, int level, int force)
{
	struct stat buf;
	int res;
	long long fsize;
	long long freeR, freeS;


	printf("cacheFile_simple \n");



	res = stat(path, &buf);

	if (!res)
		return -1;

	fsize = buf.st_size;



	freeR = getFreeSpace_simpleCache(LVL_RAM);
	freeS = getFreeSpace_simpleCache(LVL_SSD);

	if (freeR < 0 || freeS < 0)	// error
		return -1;


	return 0;
}



int getCacheLevel_simpleCache(const char* path)
{
	return LVL_NONE;
}

int initCacheControl_simpleCache(cacheControl* cc )
{

	memset(cc, 0, sizeof(cacheControl));

	cc->initCache = initCache_simpleCache;
	cc->releaseCache= releaseCache_simpleCache;

	cc->getFreeSpace = getFreeSpace_simpleCache;
	cc->getTotalSpace= getTotalSpace_simpleCache;

	cc->cacheFile = cacheFile_simpleCache;

	cc->getCacheLevel = getCacheLevel_simpleCache;


	initCachingAlgorithm_simpleAlg(&alg, cc);

	cc->fsOperations = alg.fsOperations;

	return 0;
}







static __inline__ void getPaths(const char *path, char** ssdPath, char** ramPath, char** hddPath)
{
	//TODO: use only secure string manipulators and check for buffer overflow

    *ssdPath = malloc( (strlen(ssdMountPoint)+1+strlen(path) +1 ) * sizeof(char) );
    *ramPath = malloc( (strlen(ramMountPoint)+1+strlen(path) +1 ) * sizeof(char) );
    *hddPath = malloc( (strlen(hddMountPoint)+1+strlen(path) +1 ) * sizeof(char) );

    strcpy(*ssdPath, ssdMountPoint);
    strcat(*ssdPath,"/");
    strcat(*ssdPath,path);


    strcpy(*ramPath, ramMountPoint);
    strcat(*ramPath,"/");
    strcat(*ramPath,path);


    strcpy(*hddPath, hddMountPoint);
    strcat(*hddPath,"/");
    strcat(*hddPath,path);

}


static __inline__ void getHddPath(const char *path, char** hddPath)
{
	//TODO: use only secure string manipulators and check for buffer overflow

	*hddPath = malloc( (strlen(hddMountPoint)+1+strlen(path) +1 ) * sizeof(char) );

    strcpy(*hddPath, hddMountPoint);
    strcat(*hddPath,"/");
    strcat(*hddPath,path);

}







static int getattr_simpleCache(const char *path, struct stat *stbuf)
{
	char* hddPath=NULL;

    int res;
    int result = 0;

    getHddPath(path, &hddPath);


    res = lstat(hddPath, stbuf);
    if (res == 0)					// file is in ram
    	goto finalize;

    result = -errno;

finalize:
	free(hddPath);

    return result;   				// file is not found in cache
}


static int getdir_simpleCache(const char *path, fuse_dirh_t h, fuse_dirfil_t filler)
{
	char* hddPath=NULL;

    DIR *dp;
    struct dirent *de;
    int res = 0;

    getHddPath(path, &hddPath);


    dp = opendir(hddPath);
    if (dp != NULL)
    {
		while((de = readdir(dp)) != NULL)
		{
			res = filler(h, de->d_name, de->d_type, 0);
			if(res != 0)
				break;
		}

		closedir(dp);

	}

    if (res)
    	goto finalize;


finalize:
	free(hddPath);

    return res;
}

static int fileCopy(const char* src, const char* dst )
{
	//TODO: implement a better copying method. Preferable Zero-Copy method

	// I'm using system() as a shortcut. fork & exec would be better. cp is useful because it's efficient and fast, better than something written in a hurry

	char *s = calloc( strlen(src) + strlen(dst) + 10 ,sizeof(char));

	sprintf(s, "cp %s %s", src, dst);

	int res = system(s);

	if (res != -1 )
		return 0;
	else
		return -1;
}

static int open_simpleCache(const char *path, struct fuse_file_info * fi)
{
	char* ramPath=NULL;
	char* ssdPath=NULL;
	char* hddPath=NULL;
    int res;


    int action;

    getPaths(path, &ssdPath, &ramPath, &hddPath);

    //TODO: check for write-related flags and return error if they are found



    alg.recordAccess( path, hddPath );

    action = alg.getAction( path );


    switch(action)
    {
		case COPY_HDD_RAM:
		{
			fprintf(stderr, "+++ copying from HDD to RAM \n");

			res = fileCopy(hddPath, ramPath);

			if (res < 0)
				return -1;										// an error occurred while copying

		}
		case READ_FROM_RAM:
		{
			res = open(ramPath, fi->flags);						// open the file from RAM

			if (res == -1)
				res = -errno;
			else
				fprintf(stderr, "--- opening from RAM \n");

			break;
		}



		case COPY_SSD_RAM:
		{
			fprintf(stderr, "+++ copying from SSD to RAM \n");

			res = fileCopy(ssdPath, ramPath);

			if (res < 0)
				return -1;										// an error occurred while copying


			res = open(ramPath, fi->flags);						// open the file from RAM

			if (res == -1)
				res = -errno;
			else
				fprintf(stderr, "--- opening from RAM \n");

			break;
		}




		case COPY_HDD_SSD:
		{
			fprintf(stderr, "+++ copying from HDD to SSD \n");

			res = fileCopy(hddPath, ssdPath);

			if (res < 0)
				return -1;										// an error occurred while copying

		}
		case READ_FROM_SSD:
		{
			res = open(ssdPath, fi->flags);						// open the file from SSD

			if (res == -1)
				res = -errno;
			else
				fprintf(stderr, "--- opening from SSD \n");

			break;
		}


		case READ_FROM_HDD:
		default:
		{
			res = open(hddPath, fi->flags);						// open the file on the HDD

			if (res == -1)
				res = -errno;
			else
				fprintf(stderr, "--- opening from HDD \n");

			break;
		}



    }



   	free(ramPath);
   	free(ssdPath);
   	free(hddPath);

   	if (res >=0)
   	{
   		fi->fh = res;										// return file handle

   		fprintf(stderr, "File handle = 0x%x \n", res);

   		return 0;											// success
   	}

	return res;												// error
}

static int release_simpleCache(const char *path, struct fuse_file_info *fi)
{
	close( fi->fh );

    return 0;
}

static int read_simpleCache(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int fd;
    int res;

	fprintf(stderr, "read File handle = 0x%x \n", fi->fh);

    fd = fi->fh;
    if(fd <= 0)
        return -errno;

    res = pread(fd, buf, size, offset);
    if(res == -1)
    {
    	fprintf(stderr, "err@pread \n");
        res = -errno;
    }
    else
    	fprintf(stderr, "succ@pread \n");


    return res;

}
