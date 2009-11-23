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

struct _device {
	device_hierarchy_t* hierarchy;
};

static device* device_alloc(){

	device* dev = malloc(sizeof(device));

	return dev;
}

device* device_create(device_hierarchy_t* hierarchy) {

	device* dev = device_alloc();

	dev->hierarchy = hierarchy;

	return dev;
}

void device_destroy(device* dev) {

	destroy_hierarchy(dev->hierarchy);
	free(dev);
}

char* device_get_capability(device* dev, char* name) {

	return hierarchy_get_capability(dev->hierarchy, name);
}

char* device_get_id(device* dev) {

	return devicedef_get_id(hierarchy_get_target(dev->hierarchy));
}

char* device_get_user_agent(device* dev) {

	return devicedef_get_user_agent(hierarchy_get_target(dev->hierarchy));
}

