/*
 * simpleAlgorithm.h
 *
 *  Created on: Dec 4, 2009
 *      Author: apophys
 */

#ifndef SIMPLEALGORITHM_H_
#define SIMPLEALGORITHM_H_

struct params_simpleAlg
{
	char* fsRoot;	// mounted root of cacheFS filesystem

	char* ramMountPoint;
	char* ssdMountPoint;
};

int initCachingAlgorithm_simpleAlg(cachingAlgoritm* alg);

#endif /* SIMPLEALGORITHM_H_ */
