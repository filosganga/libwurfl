/*
 * repository.c
 *
 *  Created on: 23-mar-2009
 *      Author: Filippo De Luca
 */

#include "repository.h"

#include "resource.h"
#include "utils/collection/hashmap.h"
#include "utils/collection/functors.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct _repository_t {
	const char* version;
	hashmap_t* devices;
};

static repository_t* alloc_repository() {

	repository_t* tmp = malloc(sizeof(repository_t));

	return tmp;
}

void index_devicedefs(hashtable_t* set, hashmap_t* map) {

	tomap_data_t tomap_data;
	tomap_data.key_get = &devicedef_id;
	tomap_data.map = map;

	coll_functor_t functor;
	functor.data = &tomap_data;
	functor.functor = &tomap_functor;

	hashtable_foreach(set, &functor);
}

repository_t* repository_create(const char* root, const char** patches) {

	repository_t* repository = alloc_repository();

	resource_data_t root_data;

	hashtable_options_t devices_opts = {20000,0.75f};
	root_data.devices = hashtable_create(&devicedef_eq, &devicedef_hash, &devices_opts);
	resource_parse(root, &root_data);

	repository->version = root_data.version;

	hashmap_options_t devicedefs_by_id_opts = {20000, 0.75f};
	hashmap_t* devicedefs_by_id = hashmap_create(&devicedef_eq, &devicedef_hash, &devicedefs_by_id_opts);

	index_devicedefs(root_data.devices, devicedefs_by_id);
	hashtable_destroy(root_data.devices, NULL);

	hashmap_options_t devices_by_id_opts = {20000, 0.75f};
	repository->devices = hashmap_create(&device_eq, &device_hash, &devices_by_id_opts);

	// TODO create devices
	hashmap_foreach(devicedefs_by_id, NULL);

	// destroy devicedefs
	hashmap_foreach(devicedefs_by_id, NULL);
	hashmap_destroy(devicedefs_by_id);


	return repository;
}

void repository_destroy(repository_t* repository) {

	hashmap_destroy(repository->devices);
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
