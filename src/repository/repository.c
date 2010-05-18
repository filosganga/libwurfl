/*
 * repository.c
 *
 *  Created on: 23-mar-2009
 *      Author: Filippo De Luca
 */

#include "repository.h"
#include "resource/resource-impl.h"
#include "devicedef-impl.h"
#include "hierarchy-impl.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

static int get_devices_functor(const void* item, void* data) {

	hashset_t* devices = data;
	hashset_add(devices, item);

	return 0;
}

typedef struct {
	hashmap_t* map;
	void* (*key_get)(const void* item);
} tomap_data_t;

static int tomap_functor(const void* item, void* data) {
	tomap_data_t* tomap_data = data;

	void* key = tomap_data->key_get(item);
	hashmap_put(tomap_data->map, key, item);

	return 0;
}

static void* devicedef_getkey(const void* item) {

	const devicedef_t* devicedef = item;

	return devicedef_get_id(devicedef);
}

struct _repository_t {
	char* version;
	devicedefs_t* devices;
};

static repository_t* alloc_repository(){
	repository_t* tmp = malloc(sizeof(repository_t));

	if(!tmp){
		// TODO throw error
	}

	return tmp;
}

repository_t* repository_create(resource_t* root, resource_t** patches) {

	repository_t* repository = alloc_repository();

	resource_data_t* root_data = resource_parse(root);

	//repository->version = strdup(root_data->version);
	repository->devices = root_data->devices;

	free(root_data);

	return repository;
}

void repository_destroy(repository_t* repository) {

	// TODO implement
	hashmap_destroy(repository->devices);
}

uint32_t repository_size(repository_t* repository) {

	assert(repository != NULL);

	return (uint32_t)hashmap_size(repository->devices);
}

devicedef_t* repository_get_device(repository_t* repository, char* id) {

	assert(repository!=NULL);
	assert(id != NULL);

	return hashmap_get(repository->devices, id);
}

hashset_t* repository_get_devices(repository_t* repository) {

	hashset_t* devices = hashset_create(&devicedef_hash, &devicedef_hash, &devicedef_cmp);

	coll_functor_t functor;
	functor.data = devices;
	functor.functor = &get_devices_functor;

	hashmap_foreach(repository->devices, &functor);

	return devices;
}
