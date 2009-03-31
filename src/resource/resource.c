/*
 * resource.c
 *
 *  Created on: 31-mar-2009
 *      Author: filosganga
 */

#include "resource-impl.h"

#include <stdlib.h>
#include <assert.h>


resource_t* resource_create(char* path) {

	resource_t* resource = malloc(sizeof(resource_t));
	resource->path = path;

	return resource;
}

void resource_destroy(resource_t* resource) {

	assert(resource!=NULL);
	free(resource);
}
