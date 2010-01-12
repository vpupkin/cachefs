/*
 * simpleAlgorithm.c
 *
 *  Created on: Dec 4, 2009
 *      Author: apophys
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "cachingAlgorithms.h"
#include "simpleAlgorithm.h"


static char* fsRoot=NULL;
static char* ssdMountRoot=NULL;
static char* ramMountRoot=NULL;


int initCacheStructs_simpleAlg(void* parameters)
{
	struct params_simpleAlg* p= parameters;

	printf("initCacheStructs_simpleAlg \n");

	fsRoot = strdup(p->fsRoot);
	ssdMountRoot = strdup(p->ssdMountPoint);
	ramMountRoot = strdup(p->ramMountPoint);

	return 0;
}

int releaseCacheStructs_simpleAlg()
{
	free(fsRoot);
	free(ssdMountRoot);
	free(ramMountRoot);

	return 0;
}


static __inline__ void getPaths(const char *path, char** ssdPath, char** ramPath)
{
	//TODO: use only secure string manipulators and check for buffer overflow

    *ssdPath = malloc( (strlen(ssdMountRoot)+1+strlen(path + strlen(fsRoot))  +1) * sizeof(char) );
    *ramPath = malloc( (strlen(ramMountRoot)+1+strlen(path + strlen(fsRoot))  +1) * sizeof(char) );

    strcpy(*ssdPath, ssdMountRoot);
    strcat(*ssdPath,"/");
    strcat(*ssdPath,path + strlen(fsRoot));


    strcpy(*ramPath, ramMountRoot);
    strcat(*ramPath,"/");
    strcat(*ramPath,path + strlen(fsRoot));

}

static int open_simpleAlg(const char *path, int flags)
{
    int res, res1, res2;
    char* ssdPath=NULL;
    char* ramPath=NULL;

    res=0;

    getPaths(path, &ssdPath, &ramPath);

    res1 = open(ssdPath, flags);
    res2 = open(ramPath, flags);

    if(res1 == -1 && res2== -1)  // file not found in RAM or SSD
    {
		res = -errno;
		goto finalize;
    }

    if (res1 == -1 )
    	close(res2);
    else
		close(res1);

finalize:

    free(ssdPath);
    free(ramPath);


    return res;
}

int initCachingAlgorithm_simpleAlg(cachingAlgoritm* alg)
{
	memset(alg,0,sizeof(cachingAlgoritm));

	alg->initCacheStructs = initCacheStructs_simpleAlg;
	alg->releaseCacheStructs = releaseCacheStructs_simpleAlg;


	alg->fsOperations.open = open_simpleAlg;

	return 0;
}
