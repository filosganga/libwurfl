/*
 * repository.c
 *
 *  Created on: 23-mar-2009
 *      Author: Filippo De Luca
 */

#include "repository.h"

#include "resource.h"
#include "devicedef.h"
#include "utils/hashmap.h"
#include "utils/hashtable.h"
#include "utils/functors.h"
#include "utils/utils.h"
#include "utils/logger.h"

#include <libxml/xmlstring.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct _repository_t {
	const char version[255];
	hashtable_t* strings;
	hashmap_t* devices;
	logger_t* logger;
};

// Functors
static void devicedef_undupe(void* item, const void* data) {

	devicedef_t* devicedef = item;

	hashmap_destroy(devicedef->capabilities, &coll_default_unduper, NULL);
	free(devicedef);
}

// Private functions ******************************************************

static void init_devices(repository_t* repository, const char* root_path);

static void apply_patch(repository_t* repository, const char* path);

// Implementations ********************************************************

repository_t* repository_create(const char* root, const char** patches) {

	repository_t* repository = malloc(sizeof(repository_t));
	if(!repository) {
		// FIXME Error message
		exit(-1);
	}

	hashtable_options_t strings_opts = {1000, 1.5f};
	repository->strings = hashtable_create(&string_eq, &string_hash, &strings_opts);
	repository->logger = logger_init();

	init_devices(repository, root);
	repository_patch(repository, patches);

	return repository;
}

void repository_destroy(repository_t* repository) {

	hashmap_destroy(repository->devices, &devicedef_undupe, NULL);
	hashtable_destroy(repository->strings, &coll_default_unduper, NULL);

	logger_destroy(repository->logger);

	free(repository);
}

void repository_reload(repository_t* repository, const char* root, const char** patches) {
	// TODO to implement
}

void repository_patch(repository_t* repository, const char** patches) {

	const char **patch = patches;
	while(patch && *patch) {
		apply_patch(repository, *patch);
		patch++;
	}
}

devicedef_t* repository_get(repository_t* repository, const xmlChar* id) {

	assert(repository!=NULL);
	assert(id != NULL);

	return hashmap_get(repository->devices, id);
}

uint32_t repository_size(repository_t* repository) {

	assert(repository != NULL);

	return hashmap_size(repository->devices);
}

bool repository_foreach(repository_t* repository, coll_functor_f* functor, void* functor_data) {

	assert(repository != NULL);
	assert(functor != NULL);

	return hashmap_foreach(repository->devices, functor, functor_data);
}

// Support functions ******************************************************

static void init_devices(repository_t* repository, const char* path) {

	resource_data_t root_data;

	hashmap_options_t devices_opts = {10000, 1.5f};
	root_data.devices = hashmap_create(&string_eq, &string_hash, &devices_opts);
	resource_parse(&root_data, path, repository->strings);

	repository->devices = root_data.devices;
	strcpy(repository->version, root_data.version);
}

static void apply_patch(repository_t* repository, const char* path) {

}




