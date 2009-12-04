#include <stdlib.h>
#include <stdio.h>
#include <sys/statfs.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cachefs.h"
#include "simpleCache.h"

#include "cachingAlgorithms.h"
#include "simpleAlgorithm.h"

char* ramMountPoint=NULL;
char* ssdMountPoint=NULL;

static cachingAlgoritm alg;

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

int initCache_simpleCache(int* ramSize, int* ssdSize, void* parameters)
{
	long long freeR, freeS;
	struct params_simpleCache* params= parameters;


	printf("initCache_simple\n");

	ssdMountPoint = strdup(params->ssdMountPoint);
	ramMountPoint = strdup(params->ramMountPoint);



	freeR = getFreeSpace_simpleCache(LVL_RAM);
	freeS = getFreeSpace_simpleCache(LVL_SSD);

	if (freeR < 0 || freeS < 0)	// error
		return -1;

	if (freeR < *ramSize || freeS < *ssdSize)	// insufficient space for allocation
		return -1;



	alg.initCacheStructs(&params);



	return 0;
}

int releaseCache_simpleCache()
{
	free(ssdMountPoint);
	free(ramMountPoint);


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

	cc->cacheFile = cacheFile_simpleCache;

	cc->getCacheLevel = getCacheLevel_simpleCache;


	initCachingAlgorithm_simpleAlg(&alg);

	return 0;
}
