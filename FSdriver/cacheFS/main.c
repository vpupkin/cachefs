/*
 * main.c
 *
 *  Created on: Nov 26, 2009
 *      Author: Ioan Ovidiu Hupca
 */

#ifdef linux
/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500
#endif

#define FUSE_USE_VERSION 26 /* latest fuse version */
#include <fuse.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/statfs.h>

#include "cachefs.h"
#include "simpleCache.h"


char* cachefsMountPoint=NULL;
cacheControl cc;


static struct fuse_operations *cache_oper=NULL;		// pointer to the structure describing the file operations. Filled by calling initCache

int main(int argc, char *argv[])
{


	struct params_simpleCache p={.ssdMountPoint="/ssd", .ramMountPoint="/dev/shm", .hddMountPoint="/tmp/hdd" , .cachefsMountPoint=argv[1]};

	int ramSize=128;
	int ssdSize=128;





	cachefsMountPoint = strdup(argv[1]);


	initCacheControl_simpleCache(&cc);

	cc.initCache(&ramSize, &ssdSize, &p, &cache_oper);



	fuse_main(argc, argv, cache_oper, NULL);

    return 0;

}

