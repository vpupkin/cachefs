/*
 * cachingAlgorithms.h
 *
 *  Created on: Dec 2, 2009
 *      Author: apophys
 */

#ifndef CACHINGALGORITHMS_H_
#define CACHINGALGORITHMS_H_

typedef struct _cachingAlgorithm
{
	int (*initCacheStructs)(void* structure)

	int (*cacheFile)(const char* path, int level, int force, void* structure);

	int (*uncacheFile)(const char* path, int level, void* structure);

	int (*garbageCollect)(int level, void* structure);

	int (*getCacheStatus)();

} cachingAlgoritm;


#endif /* CACHINGALGORITHMS_H_ */
