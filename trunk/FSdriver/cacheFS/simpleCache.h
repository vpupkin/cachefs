/*
 * simpleCache.h
 *
 *  Created on: Dec 2, 2009
 *      Author: apophys
 */

#ifndef SIMPLECACHE_H_
#define SIMPLECACHE_H_

#include "cachefs.h"
#include "cachingAlgorithms.h"

struct params_simpleCache
{
	char* cachefsMountPoint;

	char* ramMountPoint;
	char* ssdMountPoint;
	char* hddMountPoint;
};

int initCacheControl_simpleCache(cacheControl* cc , cachingAlgoritm* algorithm);



#endif /* SIMPLECACHE_H_ */
