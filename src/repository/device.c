/*
 * device.c
 *
 *  Created on: 24-mar-2009
 *      Author: Filippo De Luca
 */

#include "device-impl.h"

#include <stdlib.h>
#include <strings.h>
#include <assert.h>


static device_t* device_alloc(){

	device_t* dev = malloc(sizeof(device_t));
	if(dev==NULL) {
		// TODO Error
	}

	return dev;
}

device_t* device_create(const char* id, const char* user_agent, hashmap_t* capabilities, device_t* parent) {

	device_t* device = device_alloc();
	device->id = id;
	device->capabilities=capabilities;
	device->parent=parent;

	return device;
}

void device_destroy(device_t* device) {

	assert(device != NULL);

	// TODO free capabilities?
	free(device);
}

char* device_capability(device_t* device, const char* name) {

	assert(device!=NULL);
	assert(name!=NULL);
	assert(strlen(name)>0);

	device_t* current = device;
	const char* value = NULL;
	while((value = hashmap_get(current->capabilities, name)) == NULL && current->parent != NULL) {
		current = current->parent;
	}

	if(value == NULL) {
		// Error
	}

	return value;
}

char* device_id(device_t* device) {

	assert(device!=NULL);

	return device->id;
}

char* device_user_agent(device_t* device) {

	assert(device!=NULL);

	return device->user_agent;
}

