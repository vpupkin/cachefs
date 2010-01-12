/*
 * algorithms.h
 *
 *  Created on: Nov 30, 2009
 *      Author: apophys
 */

#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

#define FUSE_USE_VERSION 26 /* latest fuse version */
#include <fuse.h>

#define LVL_NONE 0
#define LVL_SSD 1
#define LVL_RAM 2

typedef struct _cacheControl
{

	int (*initCache)(int* ramSize, int* ssdSize, void* parameters, struct fuse_operations **oper);
	int (*releaseCache)();

	int (*cacheFile)(const char* path, int level, int force);



	int (*getCacheLevel)(const char* path);

	long long (*getFreeSpace)(int level);

	int (*cleanup)(int level);


	int (*getCacheState)();


	struct fuse_operations fsOperations;

} cacheControl;

#endif /* ALGORITHMS_H_ */
