/*
 * device.c
 *
 *  Created on: 24-mar-2009
 *      Author: filosganga
 */

#include "device.h"
#include "repository/devicedef.h"
#include "repository/hierarchy.h"

#include <stdlib.h>

struct _device_t {
	device_hierarchy_t* hierarchy;
};

static device_t* device_alloc(){

	device_t* dev = malloc(sizeof(device_t));

	return dev;
}

device_t* device_create(device_hierarchy_t* hierarchy) {

	device_t* dev = device_alloc();

	dev->hierarchy = hierarchy;

	return dev;
}

void device_destroy(device_t* dev) {

	destroy_hierarchy(dev->hierarchy);
	free(dev);
}

char* device_get_capability(device_t* dev, char* name) {

	return hierarchy_get_capability(dev->hierarchy, name);
}

char* device_get_id(device_t* dev) {

	return devicedef_get_id(hierarchy_get_target(dev->hierarchy));
}

char* device_get_user_agent(device_t* dev) {

	return devicedef_get_user_agent(hierarchy_get_target(dev->hierarchy));
}

