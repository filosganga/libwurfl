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
#include "normalizer.h"
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
	hashmap_t* devices;
	hashtable_t* capabilities;
	matcher_t* matcher;
	normalizer_t* normalizer;
};

/* Hidden function in interface due to threadsafe */

void wurfl_reload(wurfl_t* wurfl, const char* root, const char** patches);

void wurfl_npatch(wurfl_t* wurfl, const char** patches);

void wurfl_patch(wurfl_t* wurfl, const char* patch);


static bool patch_device(const void* item, void* xtra);

static bool normalize_device(const void* item, void* xtra);

static void free_device(void* item, const void* xtra);

static void init_devices(wurfl_t* wurfl, const char* main_path);

wurfl_t* wurfl_init(const char* main_path, const char** patch_paths) {

	wurfl_t* wurfl = malloc(sizeof(wurfl_t));
	if(wurfl==NULL) {
		error(1,errno,"error allocating memory to wurfl");
	}

	wurfl->normalizer = normalizer_init();
	wurfl->devices = hashmap_init(&string_eq, &string_hash, NULL);
	wurfl->capabilities = hashtable_init(&string_eq, &string_hash, NULL);


	init_devices(wurfl, main_path);
	wurfl_npatch(wurfl, patch_paths);

	wurfl->matcher = matcher_init(wurfl->devices);

	fprintf(stdout, "wurfl initialized with %d devices and %d capabilities\n", hashmap_size(wurfl->devices), hashtable_size(wurfl->capabilities));

	return wurfl;
}

void wurfl_free(wurfl_t* wurfl) {

	normalizer_free(wurfl->normalizer);
	matcher_free(wurfl->matcher);
	hashtable_free(wurfl->capabilities, &coll_default_unduper, NULL);
	hashmap_free(wurfl->devices, &free_device, NULL);

	free(wurfl);
}

device_t* wurfl_match(const wurfl_t* wurfl, const char* user_agent) {

	if(user_agent==NULL) {
		return NULL;
	}
	else {
		char normalized_ua[8 * 1024];
		memset(normalized_ua, '\0', 8 * 1024);
		normalizer_apply(wurfl->normalizer, normalized_ua, user_agent);

		devicedef_t* matched = matcher_match(wurfl->matcher, normalized_ua);

		return device_init(wurfl->devices, matched);
	}
}

void wurfl_reload(wurfl_t* wurfl, const char* root, const char** patches) {

	hashtable_free(wurfl->capabilities, &coll_default_unduper, NULL);
	hashmap_free(wurfl->devices, &free_device, NULL);

	init_devices(wurfl, root);
	wurfl_npatch(wurfl, patches);
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
	rdata.devices = hashmap_init(&string_eq, &string_hash, NULL);
	rdata.capabilities = wurfl->capabilities;
	parse_resource(patch, &rdata);

	hashmap_foreach_value(rdata.devices, &normalize_device, wurfl->normalizer);
	hashmap_foreach_value(rdata.devices, &patch_device, wurfl->devices);

}

size_t wurfl_size(wurfl_t* wurfl) {
	return hashmap_size(wurfl->devices);
}

size_t wurfl_capabilities_size(wurfl_t* wurfl) {
	return hashtable_size(wurfl->capabilities);
}

// Support functions ******************************************************

static void init_devices(wurfl_t* wurfl, const char* path) {

	parser_data_t rdata;
	rdata.devices = wurfl->devices;
	rdata.capabilities = wurfl->capabilities;
	parse_resource(path, &rdata);

	hashmap_foreach_value(rdata.devices, &normalize_device, wurfl->normalizer);
}

static bool patch_device(const void* item, void* xtra) {

	devicedef_t* patcher = (devicedef_t*)item;
	hashmap_t* devices = (hashmap_t*)xtra;

	devicedef_t* patching = hashmap_get(devices, patcher->id);
	if(patching) {
		devicedef_patch(patching, patcher);
	}
	else {
		hashmap_put(devices, patcher->id, patcher);
	}

	return false;
}

static bool normalize_device(const void* item, void* xtra) {

	devicedef_t* device = (devicedef_t*)item;
	normalizer_t* normalizer = (normalizer_t*)xtra;

	if(device->user_agent!=NULL) {

		char tmp[8 * 1024];
		normalizer_apply(normalizer, tmp, device->user_agent);
		if(strcmp(tmp, device->user_agent)!=0) {
			free(device->user_agent);
			device->user_agent = strdup(tmp);
		}
	}

	return false;
}

static void free_device(void* item, const void* xtra) {
	devicedef_t* devicedef = (devicedef_t*)item;

	devicedef_free(devicedef);
}
