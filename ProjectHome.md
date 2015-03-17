**Abstract**

The focus of this project is a hybrid file system, with the declared purpose of accelerating file access time and a possible improvement of power usage through the combined usage of random-access memory, solid-state drives and hard disk drives.
The idea is simple: use the fast memory (RAM and SSD) as a two-level cache. The most accessed files will be stored into RAM, while the medium accessed files will reside in the SSD. When a file is not located into the cache memory, a cache miss event takes place and data is read from the hard drive.

By using the SSD as a level 2 cache, large files can be accessed very fast (as compared to the HDD). Large files cannot typically be held into a RAM disk, as its capacity can be easily filled. Current SSD models have capacities of 120 GB and reading speeds of up to 250 MB/s. This is well beyond what a high speed hard drive can offer (Western Digital Velociraptor topping at ~100 MB/s).

Performance tuning can be done for specific applications by creating file acces profiles and then storing that data into the cache levels. Also, a library for allowing cache-aware application will be implemented.

Power usage improvement is achieved by obtaining a "stable file access profile": all the needed files are located in the RAM and SSD, therefore making it possible to shut down the hard drive in order to save power.

The inspiration for this project was the Conquest-2 project at UCLA (http://www.lasr.cs.ucla.edu/Conquest-2.html), which used only RAM and HDD, but employed a more sofisticated file access pattern profiling that what it is planned.

**Developing team:** Ioan Ovidiu Hupca, Ionuț Petre and Alexandru Stan

**Keywords:** filesystem, hybrid, cache, solid-state drive

**References:**

1. [Conquest-2, Improving Energy Efficiency and Performance Through a Disk/Hybrid File System](http://www.lasr.cs.ucla.edu/Conquest-2.html)

2. The Conquest File System: Better Performance Through a Disk/Persistent-RAM Hybrid Design [link1](http://www.cs.fsu.edu/~awang/papers/tos2006.pdf) [link2](http://usenix.org/events/fast02/wips/wang.pdf)

3. [Proceedings of the 2002 USENIX Annual Technical Conference. Conquest:  Better Performance Through A Disk/Persistent-RAM Hybrid File System](http://www.usenix.org/events/usenix02/full_papers/wang/wang.pdf)

**Current development stage:**

  * [File System driver](http://code.google.com/p/cachefs/wiki/Filesystem_driver)