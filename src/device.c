/*
 * Copyright 2011 ff-dev.org
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Written by Filippo De Luca <me@filippodeluca.com>.  */

#include "device-impl.h"

#include "devicedef.h"
#include "utils/error.h"
#include "utils/utils.h"
#include "utils/hashmap.h"

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <assert.h>
#include <errno.h>

extern int errno;

void explode_capabilities(hashmap_t* capabilities, const devicedef_t* devicedef, repository_t* repository) {

	if(devicedef->fall_back!=NULL && strlen(devicedef->fall_back)>0 && strcmp(devicedef->fall_back, "root")!=0) {
		explode_capabilities(capabilities, repository_get(repository, devicedef->fall_back), repository);
	}

	hashmap_putall(capabilities, devicedef->capabilities);
}

device_t* device_init(repository_t* repository, const devicedef_t* devicedef) {

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

char* device_capability(const device_t* device, const char* name) {

	assert(device!=NULL);
	assert(name!=NULL);

	devicedef_t* current = device->root;

	char* value = NULL;

	while((value = hashmap_get(current->capabilities, name)) == NULL && current->fall_back != NULL) {
		current = repository_get(device->repository, current->fall_back);
	}

	return value;
}

char* device_id(const device_t* device) {

	assert(device!=NULL);

	return device->root->id;
}

char* device_user_agent(const device_t* device) {

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
