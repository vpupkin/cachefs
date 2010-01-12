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


	void (*recordAccess)(const char*  path, const char* hddPath );

	int (*getAction)(const char* path );

} cachingAlgoritm;


/* Actions ordered by the caching algorithms for execution by the cache implementation */

#define COPY_HDD_RAM  0x13 		/* copy from HDD to RAM and read file from RAM */
#define COPY_SSD_RAM  0x12 		/* copy from SSD to RAM and read file from RAM */
#define COPY_HDD_SSD  0x11		/* copy from HDD to SSD and read file from SSD */

#define READ_FROM_RAM 0x02		/* read file from RAM */
#define READ_FROM_SSD 0x01		/* read file from SSD */
#define READ_FROM_HDD 0x00		/* read file from HDD */




#endif /* CACHINGALGORITHMS_H_ */
