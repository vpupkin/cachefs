/*
 * simpleSplitAlgorithm.h
 *
 *  Created on: Dec 4, 2009
 *      Author: apophys
 */

#ifndef SIMPLESPLITALGORITHM_H_
#define SIMPLESPLITALGORITHM_H_

struct params_simpleSplitAlg
{
	char* fsRoot;	// mounted root of cacheFS filesystem

	char* ramMountPoint;
	char* ssdMountPoint;
};

int initCachingAlgorithm_simpleSplitAlg(cachingAlgoritm* alg, cacheControl* ccontrol);

#endif /* SIMPLEALGORITHM_H_ */
