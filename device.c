/*
 * device.c
 *
 *  Created on: 24-mar-2009
 *      Author: filosganga
 */

#include "device.h"

#include <stdlib.h>

struct _device {
	device_hierarchy* hierarchy;
};

static device* device_alloc(){

	device* dev = malloc(sizeof(device));

	return dev;
}

device* device_create(device_hierarchy* hierarchy) {

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

	return hierarchy_get_target(dev->hierarchy)->id;
}

char* device_get_user_agent(device* dev) {

	return hierarchy_get_target(dev->hierarchy)->user_agent;
}

