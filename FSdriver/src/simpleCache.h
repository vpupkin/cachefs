/*
 * simpleCache.h
 *
 *  Created on: Dec 2, 2009
 *      Author: apophys
 */

#ifndef SIMPLECACHE_H_
#define SIMPLECACHE_H_

#include "cachefs.h"

struct params_simpleCache
{
	char* ramMountPoint;
	char* ssdMountPoint;
};

int initCacheControl_simpleCache(cacheControl* cc );



#endif /* SIMPLECACHE_H_ */
