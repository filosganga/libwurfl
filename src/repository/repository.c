/*
 * repository.c
 *
 *  Created on: 23-mar-2009
 *      Author: Filippo De Luca
 */

#include "repository.h"

#include "resource.h"
#include "device-impl.h"
#include "utils/collection/hashmap.h"
#include "utils/collection/functors.h"
#include "utils/collection/utils.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct _repository_t {
	const char* version;
	hashmap_t* devices;
};

typedef struct {
	hashmap_t* devicedefs_by_id;
	hashmap_t* devices;
} functor_create_devices_data_t;

static device_t* create_device(const char* id, hashmap_t* devicedefs, hashmap_t* devices);

static int functor_create_devices(const void* item, void* functor_data);

static repository_t* alloc_repository();

static void index_devicedefs(hashtable_t* set, hashmap_t* map);

static int enumerate(const void* item, void* data) {

	devicedef_t* devicedef = item;
	printf("Device: %s\n", devicedef->id);

	return 0;
}

repository_t* repository_create(const char* root, const char** patches) {

	repository_t* repository = alloc_repository();

	resource_data_t root_data;

	hashtable_options_t devices_opts = {20000,0.75f};
	root_data.devices = hashtable_create(&devicedef_eq, &devicedef_hash, &devices_opts);
	resource_parse(root, &root_data);

	repository->version = root_data.version;

	hashmap_options_t devicedefs_by_id_opts = {20000, 0.75f};
	hashmap_t* devicedefs_by_id = hashmap_create(&string_eq, &string_hash, &devicedefs_by_id_opts);

	index_devicedefs(root_data.devices, devicedefs_by_id);
	hashtable_destroy(root_data.devices, NULL);

	hashmap_options_t devices_by_id_opts = {20000, 0.75f};
	repository->devices = hashmap_create(&string_eq, &string_hash, &devices_by_id_opts);

	// Create devices
	functor_create_devices_data_t functor_data;
	functor_data.devicedefs_by_id = devicedefs_by_id;
	functor_data.devices = repository->devices;
	coll_functor_t functor;
	functor.data = &functor_data;
	functor.functor = &functor_create_devices;

	hashmap_foreach(devicedefs_by_id, &functor);

	// destroy devicedefs
	coll_unduper_t unduper;
	coll_init_default_unduper(&unduper);
	hashmap_destroy(devicedefs_by_id, &unduper);

	return repository;
}

void repository_destroy(repository_t* repository) {

	hashmap_destroy(repository->devices, NULL);
	free(repository);
}

device_t* repository_get_device(repository_t* repository, const char* id) {

	assert(repository!=NULL);
	assert(id != NULL);

	return hashmap_get(repository->devices, id);
}

uint32_t repository_size(repository_t* repository) {

	assert(repository != NULL);

	return (uint32_t)hashmap_size(repository->devices);
}

int repository_foreach(repository_t* repository, coll_functor_t* functor) {

	return hashmap_foreach(repository->devices, functor);
}

// Support functions ******************************************************

static device_t* create_device(const char* id, hashmap_t* devicedefs, hashmap_t* devices) {

	if(string_eq(id, "root")) {
		return NULL;
	}

	device_t* device = hashmap_get(devices, id);
	if(device==NULL) {

		devicedef_t* devicedef = hashmap_get(devicedefs, id);

		// create device
		device = malloc(sizeof(device_t));
		device->id = devicedef->id;
		device->user_agent = devicedef->user_agent;
		device->capabilities = devicedef->capabilities;
		device->parent = create_device(devicedef->fall_back, devicedefs, devices);

		// put device in the map
		hashmap_put(devices, id, device);
	}

	return device;
}

static int functor_create_devices(const void* item, void* functor_data) {

	const devicedef_t* devicedef = item;
	functor_create_devices_data_t* data = functor_data;
	hashmap_t* devicedefs_by_id = data->devicedefs_by_id;
	hashmap_t* devices_by_id = data->devices;

	device_t* device = create_device(devicedef->id, devicedefs_by_id, devices_by_id);

	return 0;
}

static repository_t* alloc_repository() {

	repository_t* tmp = malloc(sizeof(repository_t));

	return tmp;
}

static void* devicedef_id(const void* item) {

	devicedef_t* devicedef = item;

	return devicedef->id;
}

static void index_devicedefs(hashtable_t* set, hashmap_t* map) {

	functor_tomap_data_t tomap_data;
	tomap_data.key_get = &devicedef_id;
	tomap_data.map = map;

	coll_functor_t functor;
	functor_init(&functor_tomap, &tomap_data, &functor);

	hashtable_foreach(set, &functor);
}

