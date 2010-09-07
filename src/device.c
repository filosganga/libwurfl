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

char* device_capability(const device_t* device, const char* name) {

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

char* device_id(const device_t* device) {

	assert(device!=NULL);

	return device->id;
}

char* device_user_agent(const device_t* device) {

	assert(device!=NULL);

	return device->user_agent;
}

void device_patch(device_t* patched, const device_t* source, const device_t* patching) {

	// It is not relevant
	patched->id = source->id;
	patched->user_agent = source->user_agent;
	patched->parent = patching->parent;

	hashmap_putall(patched->capabilities, source->capabilities);
	hashmap_putall(patched->capabilities, patching->capabilities);
}

int device_cmp(const void* left, const void* right) {

	assert(left!=NULL);
	assert(right!=NULL);

	device_t* ldevice = left;
	device_t* rdevice = right;

	return strcmp(ldevice->id, rdevice->id);
}

uint32_t device_hash(const void* item) {

	assert(item != NULL);

	device_t* device = item;

	return string_hash(device->id);
}

bool device_eq(const void* left, const void* right) {

	assert(left!=NULL);
	assert(right!=NULL);


	device_t* ldevice = left;
	device_t* rdevice = right;

	return string_eq(ldevice->id, rdevice->id);
}
