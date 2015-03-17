# Application Architecture #

![http://cachefs.googlecode.com/files/arhitectura%20aplicatie%201.png](http://cachefs.googlecode.com/files/arhitectura%20aplicatie%201.png)

# Deliverables #
  1. filesystem driver
  1. cacheFS user control library
  1. application profile builder
  1. cache algorithms simulator

## Filesystem driver ##

The Filesystem driver is the core module of the project. Its task is to supply files from the three available data stores (by speed: RAM, SSD and HDD) and apply different caching and prefetching algoritms for moving data between the data stores.

Its main purpose is to provide accelerated file access. Because of this and the inherent complexity of the write propagation task, the filesystem will be readonly. Since one of the targets for this project is a web server, which is usually not allowed write access to the files it serves, we deem this to be acceptable at this stage. It can also be viewed as a security mechanism.

For moving data between the data stores, different algoritms will be implemented. They will be used according to commands specified from user space (either by the user via command line, through application profiles, by cacheFS-aware applications via the user control library) or self-selected by the driver.

By loading an application profile, the driver can preload a given number of files so they can be readily available when the application requests them.

## cacheFS user control library ##

The development and testing of the driver will require the creation of user-space control functions. It is therefore simple to augment and expose these functions to the user via a control library.

For example, functions like fopen() can be augmented by adding a parameter specifying the data store level requested (preload file to RAM, SSD or load it normally from the HDD). A fprefetch() can preload a file in a give data store level, making it available at a later date to the application, decreasing wait time and increasing read speed.

## Application profile builder ##

In order for the caching and prefetching algoritms to work optimally for a given application, an application profile can be used.

The application profile will indicate what files are accessed, when they are accessed and in what sequence the files are usually loaded. By marking sequence start points, the driver can preload the files that are next in the sequence before the request for them actually arrives.

The application profile builder will be tasked with creating and analyzing the file accesses of a given application and create a task list for the driver, in the form of an application profile. It will contain lists of files to preload and the triggers for the preload. The caching algorithms usable can be specified.

This profile will be followed by the driver like a map. No analysis will be done.

## Cache algorithms simulator ##

The algorithms used for moving the data files around the data stores are paramount to providing the speed improvement desired. Therefore, careful simulations of a variety of algorithms on a variety of test cases must be conducted.

This application will be tasked with simulating all the operations conducted by the driver (moving files around the data store levels, what to do when the RAM or SSD are full etc.). The test cases can also be the initial application profiles, so the right caching and prefetching algorithms can be selected and embedded in the profile.

Different algorithms will be tested and their results will be synthesized and presented.

# Infrastructure used #

The target infrastructured will be composed of a computer having one or more hard-disk drives, a solid-state drive acting as a Level 2 cache and (obviously) RAM.

# Software environment #

  * Linux operating system, due to its open source kernel and tools
  * C language for driver development
  * C/C++/Java language for simulator and profiler
  * Tools for profile creation: ....
  * Libraries used: ...

# Related work #
  * Linux kernel RAMFS, VFS
  * sample VFS drivers
  * ...

# Team work #

  * Ioan Ovidiu Hupca: filesystem driver and user library
  * Bogdan Petre: application profiler (?)
  * Alexandru Stan: cache and prefetch algorithm simulator (?)

# Research goals #
  * improving file access latency and speed by using a hierarchical data store
  * example target: web/file servers

# Project schedule #
> blizzard style
