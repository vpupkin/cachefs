/*
 * simpleAlgorithm.c
 *
 *  Created on: Dec 4, 2009
 *      Author: apophys
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cachingAlgorithms.h"
#include "simpleAlgorithm.h"


static char* fsRoot=NULL;


int initCacheStructs_simpleAlg(void* parameters)
{
	struct params_simpleAlg* p= parameters;

	printf("initCacheStructs_simpleAlg \n");

	fsRoot = strdup(p->fsRoot);

	return 0;
}

int releaseCacheStructs_simpleAlg()
{
	free(fsRoot);

	return 0;
}

int initCachingAlgorithm_simpleAlg(cachingAlgoritm* alg)
{
	memset(alg,0,sizeof(cachingAlgoritm));

	alg->initCacheStructs = initCacheStructs_simpleAlg;
	alg->releaseCacheStructs = releaseCacheStructs_simpleAlg;

	return 0;
}
