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

#include "repository.h"

#include "parser.h"
#include "devicedef.h"
#include "utils/hashmap.h"
#include "utils/hashtable.h"
#include "utils/functors.h"
#include "utils/utils.h"
#include "utils/error.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

extern int errno;

struct _repository_t {
	const char* version;
	hashtable_t* strings;
	hashmap_t* devices;
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
		error(1,errno, "error allocating repository");
	}

	hashtable_options_t strings_opts = {1000, 1.5f};
	repository->strings = hashtable_create(&string_eq, &string_hash, &strings_opts);

	init_devices(repository, root);
	repository_patch(repository, patches);

	return repository;
}

void repository_destroy(repository_t* repository) {

	hashmap_destroy(repository->devices, &devicedef_undupe, NULL);
	hashtable_destroy(repository->strings, &coll_default_unduper, NULL);

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

devicedef_t* repository_get(repository_t* repository, const char* id) {

	assert(repository!=NULL);
	assert(id != NULL);

	return hashmap_get(repository->devices, id);
}

size_t repository_size(repository_t* repository) {

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
	parser_parse(path, repository->strings, &root_data);

	repository->devices = root_data.devices;
	repository->version = root_data.version;
}

static void apply_patch(repository_t* repository, const char* path) {

}




