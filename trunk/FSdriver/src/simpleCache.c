#include <stdlib.h>
#include <stdio.h>
#include <sys/statfs.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cachefs.h"

char* ramMountPoint=NULL;
char* ssdMountPoint=NULL;

int getFreeSpace_simple(int level)
{
	struct statfs buf;
	int res;

	switch (level)
	{
		case LVL_RAM:	res = statfs(ramMountPoint, &buf); break;
		case LVL_SSD:	res = statfs(ssdMountPoint, &buf); break;
		default:		return -1;
	}

	if (res !=0)	// error
		return -1;

	return buf.f_bsize * buf.f_bavail;	// return free space

}

int initCache_simple(int* ramSize, int* ssdSize)
{
	int freeR, freeS;

	printf("initCache_simple\n");


	freeR = getFreeSpace_simple(LVL_RAM);
	freeS = getFreeSpace_simple(LVL_SSD);

	if (freeR < 0 || freeS < 0)	// error
		return -1;

	if (freeR < *ramSize || freeS < *ssdSize)	// insufficient space for allocation
		return -1;



	return 0;
}

int cacheFile_simple(const char* path, int level, int force)
{
	struct stat buf;
	int res;
	long fsize;
	int freeR, freeS;


	printf("cacheFile_simple \n");



	res = stat(path, &buf);

	if (!res)
		return -1;

	fsize = buf.st_size;



	freeR = getFreeSpace_simple(LVL_RAM);
	freeS = getFreeSpace_simple(LVL_SSD);

	if (freeR < 0 || freeS < 0)	// error
		return -1;


	return 0;
}



int getCacheLevel_simple(const char* path)
{
	return LVL_NONE;
}

int initCacheControl_simple(cacheControl* cc )
{
	memset(cc, 0, sizeof(cacheControl));

	cc->initCache = initCache_simple;

	cc->getFreeSpace = getFreeSpace_simple;

	cc->cacheFile = cacheFile_simple;

	cc->getCacheLevel = getCacheLevel_simple;

	return 0;
}
