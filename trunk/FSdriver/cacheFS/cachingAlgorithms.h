/*
 * cachingAlgorithms.h
 *
 *  Created on: Dec 2, 2009
 *      Author: apophys
 */

#ifndef CACHINGALGORITHMS_H_
#define CACHINGALGORITHMS_H_

#define FUSE_USE_VERSION 26 /* latest fuse version */
#include <fuse.h>

typedef struct _cachingAlgorithm
{
	int (*initCacheStructs)(void* parameters);
	int (*releaseCacheStructs)();


	int (*cacheFile)(const char* path, int level, int force);
	int (*uncacheFile)(const char* path, int level);
	int (*moveFile)(const char* path, int srcLevel, int destLevel);

	int (*garbageCollect)(int level);

	int (*getCacheLevel)(const char* path);

	int (*getCacheStatus)();

	struct fuse_operations fsOperations;


} cachingAlgoritm;



#endif /* CACHINGALGORITHMS_H_ */
