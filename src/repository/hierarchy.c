/*
 * devicehierarchy.c
 *
 *  Created on: 28-mar-2009
 *      Author: filosganga
 */

#include "hierarchy-impl.h"

#include "utils/memory/allocator.h"
#include "commons.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct _device_hierarchy_t {
	linkedlist_t* devices;
};

device_hierarchy_t* create_hierarchy(linkedlist_t* devices) {

	assert(devices!=NULL);
	assert(linkedlist_size(devices) > 0);

	devicedef_t* root = (devicedef_t*)linkedlist_get(devices,0);
	assert(strcmp(devicedef_get_id(root),"generic")==0);

	device_hierarchy_t* hierarchy = malloc(sizeof(device_hierarchy_t));
	hierarchy->devices = linkedlist_create(&devicedef_eq);

	return hierarchy;
}

void destroy_hierarchy(device_hierarchy_t* hierarchy) {

	assert(hierarchy!=NULL);

	linkedlist_destroy(hierarchy->devices);
	free(hierarchy);
}

int hierarchy_get_size(device_hierarchy_t* hierarchy) {

	assert(hierarchy!=NULL);

	return linkedlist_size(hierarchy->devices);
}

char* hierarchy_get_capability(device_hierarchy_t* hierarchy, char* name) {

	assert(hierarchy!=NULL);
	assert(name!=NULL);
	assert(strlen(name)>0);

	int i=0;
	char* value = NULL;

	// TODO iterate
	// TODO if not defined throw error

	return value;
}

devicedef_t* hierarchy_get_target(device_hierarchy_t* hierarchy) {

	assert(hierarchy!=NULL);
	return linkedlist_get(hierarchy->devices, linkedlist_size(hierarchy->devices)-1);
}

devicedef_t* hierarchy_get_device(device_hierarchy_t* hierarchy, u_int32_t index) {

	assert(hierarchy!=NULL);
	assert(index< linkedlist_size(hierarchy->devices));

	return linkedlist_get(hierarchy->devices, index);
}
