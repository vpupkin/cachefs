/*
 * algorithms.h
 *
 *  Created on: Nov 30, 2009
 *      Author: apophys
 */

#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

#define LVL_NONE 0
#define LVL_SSD 1
#define LVL_RAM 2

typedef struct _cacheControl
{

	int (*initCache)(int* ramSize, int* ssdSize, void* parameters);
	int (*releaseCache)();

	int (*cacheFile)(const char* path, int level, int force);



	int (*getCacheLevel)(const char* path);

	long long (*getFreeSpace)(int level);

	int (*cleanup)(int level);


	int (*getCacheState)();

} cacheControl;

#endif /* ALGORITHMS_H_ */
