/*
 * repository.c
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */

#include "repository.h"
#include "resource-impl.h"
#include "devicedef-impl.h"
#include "hierarchy-impl.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

typedef struct tomap_data_t {
	hashmap_t* map;
	void* (*key_get)(const void* item);
} tomap_data_t;

static int tomap_functor(const void* item, void* data) {
	tomap_data_t* tomap_data = data;

	void* key = tomap_data->key_get(item);
	hashmap_put(tomap_data->map, key, item);

	return 0;
}

static void devicedef_getkey(const void* item) {

	devicedef_t* devicedef = item;

	return devicedef_get_id(devicedef);
}

struct _repository_t {
	char* version;
	hashmap_t* devices;
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
	hashset_t* devices = root_data->devices;

	printf("Devices: %d\n", hashset_size(devices));

	tomap_data_t tomap_data;
	tomap_data.key_get = &devicedef_getkey;
	tomap_data.map = hashmap_create(&string_hash, &string_equals);

	hashset_foreach(devices, &tomap_functor, &tomap_data);
	repository->devices = tomap_data.map;

	printf("Map: %d\n", hashmap_size(repository->devices));


	hashset_destroy(devices);
	free(root_data);

	return repository;
}

u_int32_t repository_size(repository_t* repository) {

	assert(repository != NULL);

	return (u_int32_t)hashmap_size(repository->devices);
}

devicedef_t* repository_get_device(repository_t* repository, char* id) {

	assert(repository!=NULL);
	assert(id != NULL);

	return hashmap_get(repository->devices, id);
}


