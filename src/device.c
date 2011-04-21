/*
 * device.c
 *
 *  Created on: 24-mar-2009
 *      Author: Filippo De Luca
 */

#include "device-impl.h"

#include "devicedef.h"
#include "utils/error.h"
#include "utils/utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <assert.h>
#include <errno.h>

extern int errno;

device_t* device_create(repository_t* repository, const devicedef_t* devicedef) {

	assert(repository!=NULL);
	assert(devicedef!=NULL);

	device_t* device = malloc(sizeof(device_t));
	if(device==NULL) {
		error(1, errno, "error allocating device");
	}

	device->root = devicedef;
	device->repository = repository;

	return device;
}

void device_release(device_t* device) {

	free(device);
}

unsigned char* device_capability(const device_t* device, const unsigned char* name) {

	assert(device!=NULL);
	assert(name!=NULL);

	devicedef_t* current = device->root;

	unsigned char* value = NULL;

	while((value = hashmap_get(current->capabilities, name)) == NULL && current->fall_back != NULL) {
		current = repository_get(device->repository, current->fall_back);
	}

	return value;
}

unsigned char* device_id(const device_t* device) {

	assert(device!=NULL);

	return device->root->id;
}

unsigned char* device_user_agent(const device_t* device) {

	assert(device!=NULL);

	return device->root->user_agent;
}

int device_cmp(const void* left, const void* right) {

	assert(left!=NULL);
	assert(right!=NULL);

	device_t* ldevice = left;
	device_t* rdevice = right;

	return strcmp(ldevice->root->id, rdevice->root->id);
}

uint32_t device_hash(const void* item) {

	assert(item != NULL);

	device_t* device = item;

	return string_hash(device->root->id);
}

bool device_eq(const void* left, const void* right) {

	assert(left!=NULL);
	assert(right!=NULL);

	device_t* ldevice = left;
	device_t* rdevice = right;

	return string_eq(ldevice->root->id, rdevice->root->id);
}
