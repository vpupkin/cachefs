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
char* fsRoot=NULL;

static cachingAlgoritm alg;


static int getattr_simpleCache(const char *path, struct stat *stbuf);
static int getdir_simpleCache(const char *path, fuse_dirh_t h, fuse_dirfil_t filler);
static int getdir_simpleCache2(const char *path, fuse_dirh_t h, fuse_dirfil_t filler);
static int open_simpleCache(const char *path, struct fuse_file_info *fi);
static int release_simpleCache(const char *path, struct fuse_file_info *);
static int read_simpleCache(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);


struct fuse_operations oper=
{
	    .getattr = getattr_simpleCache,

	    .getdir	 = getdir_simpleCache2, 	// TODO: deprecated, replace with readdir

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

int initCache_simpleCache(int* ramSize, int* ssdSize, void* parameters , struct fuse_operations **operations)
{
	long long freeR, freeS;
	struct params_simpleCache* params= parameters;
	struct params_simpleAlg pAlg;

	printf("initCache_simple\n");

	ssdMountPoint = strdup(params->ssdMountPoint);
	ramMountPoint = strdup(params->ramMountPoint);
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

	cc->cacheFile = cacheFile_simpleCache;

	cc->getCacheLevel = getCacheLevel_simpleCache;


	initCachingAlgorithm_simpleAlg(&alg);

	cc->fsOperations = alg.fsOperations;

	return 0;
}







static __inline__ void getPaths(const char *path, char** ssdPath, char** ramPath)
{
	//TODO: use only secure string manipulators and check for buffer overflow

    *ssdPath = malloc( (strlen(ssdMountPoint)+1+strlen(path) +1 ) * sizeof(char) );
    *ramPath = malloc( (strlen(ramMountPoint)+1+strlen(path) +1 ) * sizeof(char) );

    strcpy(*ssdPath, ssdMountPoint);
    strcat(*ssdPath,"/");
    strcat(*ssdPath,path);


    strcpy(*ramPath, ramMountPoint);
    strcat(*ramPath,"/");
    strcat(*ramPath,path);

}







static int getattr_simpleCache(const char *path, struct stat *stbuf)
{
	char* ramPath=NULL;
	char* ssdPath=NULL;
    int res;
    int result = 0;

    getPaths(path, &ssdPath, &ramPath);


    res = lstat(ramPath, stbuf);
    if (res == 0)					// file is in ram
    	goto finalize;

    res = lstat(ssdPath, stbuf);
    if (res == 0)					// file is in ssd
    	goto finalize;;

    fprintf(stderr, "File '%s' is not found in cache\n", path);

    result = -errno;

finalize:
	free(ramPath);
	free(ssdPath);

    return result;   				// file is not found in cache
}


static int getdir_simpleCache(const char *path, fuse_dirh_t h, fuse_dirfil_t filler)
{
	char* ramPath=NULL;
	char* ssdPath=NULL;

    DIR *dp;
    struct dirent *de;
    int res = 0;

    getPaths(path, &ssdPath, &ramPath);

    //TODO: remove duplicates


    dp = opendir(ramPath);
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

    dp = opendir(ssdPath);
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

finalize:
	free(ramPath);
	free(ssdPath);

    return res;
}

struct dirset
{
	char* d_name;
	unsigned char d_type;
};

static int getdir_simpleCache2(const char *path, fuse_dirh_t h, fuse_dirfil_t filler)
{
	char* ramPath=NULL;
	char* ssdPath=NULL;

	// self-extending string array
	struct dirset* set = NULL;
	int set_step = 100;
	int set_size = set_step;
	int set_i = 0;

	// dir enumerator data structures
    DIR *dp;
    struct dirent *de;
    int res = 0;
    int i;


    set = (struct dirset*) calloc(set_size,sizeof(struct dirset));					// init array



    getPaths(path, &ssdPath, &ramPath);


    dp = opendir(ramPath);
    if (dp != NULL)
    {
		while((de = readdir(dp)) != NULL)
		{
			res = filler(h, de->d_name, de->d_type, 0);
			if(res != 0)
				break;


			// add filename to set

			if (set_i >= set_size)
			{
				// extend
				set_size += set_step;
				set = (struct dirset*) realloc(set, set_size * sizeof(struct dirset));
			}
			set[set_i].d_name = strdup(de->d_name);
			set[set_i].d_type = de->d_type;
			set_i++;

		}

		closedir(dp);

	}

    if (res)
    	goto finalize;

    dp = opendir(ssdPath);
    if (dp != NULL)
    {
		while((de = readdir(dp)) != NULL)
		{
			int duplicate = 0;
			int i;

			// search for a duplicated filename present in RAM

			for (i=0; i<set_i; i++)
				if (strcmp(de->d_name, set[i].d_name) == 0)
				{
					duplicate = 1;
					break;
				}

			if (duplicate)
				continue;

			// no duplicate found, add file to dir listing

			res = filler(h, de->d_name, de->d_type, 0);
			if(res != 0)
				break;


		}

		closedir(dp);



	}

finalize:
	free(ramPath);
	free(ssdPath);

	for (i=0; i<set_i; i++)
		free(set[i].d_name);

	free(set);

    return res;
}


static int open_simpleCache(const char *path, struct fuse_file_info * fi)
{
	char* ramPath=NULL;
	char* ssdPath=NULL;
    int res;

    getPaths(path, &ssdPath, &ramPath);

    //TODO: check for write-related flags and return error if they are found

    res = open(ramPath, fi->flags);		// open the file in the RAM cache
    if (res != -1)
    	goto finalize;

    res = open(ssdPath, fi->flags);		// open the file in the SSD cache
    if (res != -1)
    	goto finalize;

    res = -errno;						// unable to open file


finalize:
   	free(ramPath);
   	free(ssdPath);

   	if (res >=0)
   	{
   		fi->fh = res;					// return file handle

   		fprintf(stderr, "File handle = 0x%x \n", res);

   		return 0;						// success
   	}

	return res;							// error
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
