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

#include "wurfl.h"

#include "parser.h"
#include "matcher.h"
#include "device-impl.h"
#include "devicedef.h"
#include "utils/utils.h"
#include "utils/functors.h"
#include "utils/error.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

extern int errno;

struct _wurfl_t {
	matcher_t* matcher;
	hashmap_t* devices;
	hashtable_t* capabilities;
	char* version;
};

static bool patch_device(const void* item, void* xtra);

static void init_devices(wurfl_t* wurfl, const char* root_path);

wurfl_t* wurfl_init(const char* root, const char** patches) {

	wurfl_t* wurfl = malloc(sizeof(wurfl_t));
	if(wurfl==NULL) {
		error(1,errno,"error allocating memory to wurfl");
	}

	wurfl->devices = hashmap_init(&string_eq, &string_hash, NULL);
	wurfl->capabilities = hashtable_init(&string_eq, &string_hash, NULL);

	init_devices(wurfl, root);
	//wurfl_npatch(wurfl, patches);

	wurfl->matcher = matcher_init(wurfl->devices);

	return wurfl;
}

void wurfl_free(wurfl_t* wurfl) {

	matcher_free(wurfl->matcher);
	hashmap_free(wurfl->devices, NULL, NULL);

	free(wurfl);
}

device_t* wurfl_match(const wurfl_t* wurfl, const char* user_agent) {

	devicedef_t* matched = matcher_match(wurfl->matcher, user_agent);

	return device_init(wurfl->devices, matched);
}

void wurfl_reload(wurfl_t* wurfl, const char* root, const char** patches) {
	// TODO to implement
}

void wurfl_npatch(wurfl_t* wurfl, const char** patches) {

	const char **patch = patches;
	while(patch && *patch) {
		wurfl_patch(wurfl, *patch);
		patch++;
	}
}

void wurfl_patch(wurfl_t* wurfl, const char* patch) {

	parser_data_t rdata;
	hashmap_options_t devices_opts = {1024, 1.5f};
	rdata.devices = hashmap_init(&string_eq, &string_hash, &devices_opts);
	parser_parse(patch, &rdata);

	hashmap_foreach_value(rdata.devices, &patch_device, wurfl->devices);
}

size_t wurfl_size(wurfl_t* wurfl) {
	return hashmap_size(wurfl->devices);
}

// Support functions ******************************************************

static void init_devices(wurfl_t* wurfl, const char* path) {

	parser_data_t rdata;
	rdata.devices = wurfl->devices;
	rdata.capabilities = wurfl->capabilities;
	parser_parse(path, &rdata);
}

static bool patch_device(const void* item, void* xtra) {

	hashmap_t* devices = (hashmap_t*)xtra;
	devicedef_t* patcher = (devicedef_t*)item;

	devicedef_t* patching = hashmap_get(devices, patcher->id);
	if(patching) {
		devicedef_patch(patching, patcher);
		devicedef_free(patcher);
	}
	else {
		hashmap_put(devices, patcher->id, patcher);
	}

	return false;
}
