/*
 * repository.c
 *
 *  Created on: 23-mar-2009
 *      Author: Filippo De Luca
 */

#include "repository.h"
#include "resource-impl.h"

#include "devicedef-impl.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct _repository_t {
	const char* version;
	hashmap_t* devices;
};

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

static repository_t* alloc_repository() {

	repository_t* tmp = malloc(sizeof(repository_t));

	return tmp;
}

repository_t* repository_create(const char* root, const char** patches) {

	repository_t* repository = alloc_repository();

	resource_data_t root_data = resource_parse(root);

	repository->version = root_data.version;
	repository->devices = root_data.devices;

	return repository;
}

void repository_destroy(repository_t* repository) {

	hashmap_destroy(repository->devices);
	free(repository);
}

devicedef_t* repository_get_device(repository_t* repository, const char* id) {

	assert(repository!=NULL);
	assert(id != NULL);

	return hashmap_get(repository->devices, id);
}

uint32_t repository_size(repository_t* repository) {

	assert(repository != NULL);

	return (uint32_t)hashmap_size(repository->devices);
}

int repository_foreach(repository_t* repository, coll_functor_t* functor) {

	hashmap_foreach(repository->devices, functor);
}


