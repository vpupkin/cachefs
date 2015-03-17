# Introduction #

The Filesystem driver is the core module of the project. Its task is to supply files from the three available data stores (by speed: RAM, SSD and HDD) and apply different caching and prefetching algoritms for moving data between the data stores.

The first stage of development, allowing for easier testing and debugging, is by using FUSE and running the filesystem routines in user space. Different caching algorithms will be implemented. **These will be the deliverables for the SOA 2009-2010 course.**

The second stage is a kernel module implementing an optimized variant of the filesystem developed in the first stage.

# Details - First stage #

A FUSE filesystem is used in this stage. When mounted, it provides a virtual view of the cached files.

The internal structures mantain details regarding the cache level (either SSD or RAM) as well as metadata that allows for further optimization.

The architecture is modular, allowing for plug and play modules and algorithms: there is an interface describing the caching operations and one for the caching algorithms. This allows for multiple, easily switchable, implementations of both.

## Cache control interface (cachefs.h) ##

This interface is implemented by each **cache implementation module**.

  * `int initCache(int* ramSize, int* ssdSize, void* parameters)` - initializes the filesystem, allocated RAM and SSD space and internal structures

  * `int releaseCache()` - releases the allocated space and cleanups the internal structures

  * `int cacheFile(const char* path, int level, int force)` - cache a file on the HDD. Can force a target level (RAM or SSD) or allow for it to be automatically selected by the current algorithm
  * `int getCacheLevel(const char* path)` - query the cache level of a file

  * `long long getFreeSpace(int level)` - query for cache filesystem free space

  * `int cleanup(int level)` - force a call to the cache garbage collector

  * `int getCacheState()` - query current cache filesystem state

## Caching algorithms interface (cachingAlgorithms.h) ##

This interface is implemented by each **caching algorithm**

  * `initCacheStructs(void* parameters)` - initialize caching algorithm internal structures
  * `releaseCacheStruct()` - cleanup caching algorithm internal structures

  * `cacheFile(const char* path, int level, int force)` - cache a file on the HDD. Can force a target level (RAM or SSD) or allow for it to be automatically selected
  * `uncacheFile(const char* path, int level)` - remove a file from the cache
  * `moveFile(const char* path, int srcLevel, int destLevel)` - move a file from one cache level to the other

  * `garbageCollect(int level)` - call garbage collector (try to free some space on the filesystem by applying different rules)
  * `*getCacheLevel(const char* path)` - query the cache level of a file

  * `*getCacheStatus()` - query current cache filesystem state


## Cache implementation modules ##

These modules represent the implementation of the filesystem operations. They also contain wrappers for the routines of each algorithm.

### Simple Cache ###

Represents the simplest possible implementation. It uses /dev/shm for RAM storage and another mounted partitions as the SSD (ex: /mnt/ssd). It is mostly a wrapper around the existing file system calls. Basically, when files a copied to the cache, they are copied to one of the two mount points.

### Raw Cache ###

Like Simple Cache, it uses /dev/shm and a mounted partition. But it allocates a large single file on each of them and manages the structures internally. It is useful for large files. **To be implemented**

### more to come... ###


## Caching algorithms ##

The caching algorithms control the way the RAM and SSD caches are filled, how a cache level is selected for a file and how the garbage collector works.

### Simple Algorithm ###

As the name says, it is a simple algorithm. It simply moves files to the level specified by a rule (file size for example). It does not make any optimizations during operation.

### more to come ... ###
.

# Details - Second stage #

# Deliverables #

The current source code for the driver can be found at: http://code.google.com/p/cachefs/source/browse/#svn/trunk/FSdriver/cacheFS (online browsing link)