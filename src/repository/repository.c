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
#include "utils/collection/hashtable.h"
#include "utils/collection/functors.h"
#include "utils/collection/utils.h"
#include "utils/logger.h"

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

static device_t* create_device(hashmap_t* devices, const char* id, hashmap_t* sources);

static device_t* patch_device(hashmap_t* patcheds, const char* id, hashmap_t* patchers, hashmap_t* patchings);

// Functors
typedef struct {
	hashmap_t* devicedefs_by_id;
	hashmap_t* devices;
} functor_create_devices_data_t;

static int functor_create_devices(const void* item, void* functor_data);

typedef struct {
	hashmap_t* patchers;
	hashmap_t* patchings;
	hashmap_t* patcheds;
} functor_patch_devices_data_t;

static int functor_patch_devices(const void* item, void* functor_data);

static int functor_collect_deviceid(const void *item, void *data);

static int functor_collect_devicedefid(const void *item, void *data);


static void index_devicedefs(hashtable_t* set, hashmap_t* map);

static void init_devices(repository_t* repository, const char* root_path);

static void apply_patch(repository_t* repository, const char* path);

void device_undupe(void* item, const void* data);

// Implementations ********************************************************

repository_t* repository_create(const char* root, const char** patches) {

	repository_t* repository = malloc(sizeof(repository_t));

	hashtable_options_t strings_opts = {1000, 1.5f};
	repository->strings = hashtable_create(&string_eq, &string_hash, &strings_opts);
	repository->logger = logger_init();

	init_devices(repository, root);

	repository_patch(repository, patches);

	return repository;
}

void repository_destroy(repository_t* repository) {

	coll_unduper_t device_unduper;
	device_unduper.undupe = &device_undupe;
	hashmap_destroy(repository->devices, &device_unduper);

	coll_unduper_t unduper;
	coll_init_default_unduper(&unduper);
	hashtable_destroy(repository->strings, &unduper);
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

device_t* repository_get(repository_t* repository, const char* id) {

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

static int functor_create_devices(const void* item, void* functor_data) {

	const char* id = item;
	functor_create_devices_data_t* data = functor_data;
	hashmap_t* sources = data->devicedefs_by_id;
	hashmap_t* createds = data->devices;

	create_device(createds, id, sources);

	return 0;
}

static int functor_patch_devices(const void* item, void* functor_data) {

	const char* id = item;
	functor_patch_devices_data_t* data = functor_data;
	hashmap_t* patchers = data->patchers;
	hashmap_t* patcheds = data->patcheds;
	hashmap_t* patchings = data->patchings;

	patch_device(patcheds, id, patchers, patchings);

	return 0;
}

static int functor_collect_deviceid(const void *item, void *data) {

	const device_t* device = item;
	hashtable_t* ids = data;
	hashtable_add(ids, device->id);

	return 0;
}

static int functor_collect_devicedefid(const void *item, void *data) {

	const devicedef_t* devicedef = item;
	hashtable_t* ids = data;
	hashtable_add(ids, devicedef->id);

	return 0;
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

static void init_devices(repository_t* repository, const char* path) {

	resource_data_t root_data;

	hashtable_options_t sources_opts = {10000, 1.5f};
	root_data.devices = hashtable_create(&devicedef_eq, &devicedef_hash, &sources_opts);
	resource_parse(&root_data, path, repository->strings);

	// init repository
	hashmap_options_t devices_opts = {hashtable_size(root_data.devices) * 0.2, 0.2f};
	repository->devices = hashmap_create(&string_eq, &string_hash, &devices_opts);

	// Create devices
	hashmap_options_t sourcesbyid_opts = {hashtable_size(root_data.devices) * 0.2, 0.2f};
	hashmap_t* sourcesbyid = hashmap_create(&string_eq, &string_hash, &sourcesbyid_opts);
	index_devicedefs(root_data.devices, sourcesbyid);
	hashtable_destroy(root_data.devices, NULL);

	// Create the device id set
	hashtable_options_t devicesid_opts =  {10000, 1.5f};
	hashtable_t* devicesid = hashtable_create(&string_eq, &string_hash, &devicesid_opts);
	coll_functor_t collect_devicedefid = {&functor_collect_devicedefid, devicesid};
	hashmap_foreach(sourcesbyid, &collect_devicedefid);

	// Create devices
	functor_create_devices_data_t functor_data;
	functor_data.devicedefs_by_id = sourcesbyid;
	functor_data.devices = repository->devices;
	coll_functor_t functor;
	functor.data = &functor_data;
	functor.functor = &functor_create_devices;

	// modify the repository
	hashtable_foreach(devicesid, &functor);
	strcpy(repository->version, root_data.version);

	// destroy sources
	coll_unduper_t unduper;
	coll_init_default_unduper(&unduper);
	hashmap_destroy(sourcesbyid, &unduper);
}

static void apply_patch(repository_t* repository, const char* path) {

	resource_data_t patch_data;

	logger_debug(repository->logger, __FILE__, __LINE__, "Applying %s patch", path);

	hashtable_options_t devices_opts = {10000, 1.5f};
	patch_data.devices = hashtable_create(&devicedef_eq, &devicedef_hash, &devices_opts);
	resource_parse(&patch_data, path, repository->strings);

	hashmap_options_t patched_opts = {repository_size(repository) * 0.5, 0.5f};
	hashmap_t* patcheds = hashmap_create(&string_eq, &string_hash, &patched_opts);

	hashmap_options_t patcher_opts = {hashtable_size(patch_data.devices), 0.5f};
	hashmap_t* patchers = hashmap_create(&string_eq, &string_hash, &patcher_opts);

	index_devicedefs(patch_data.devices, patchers);

	// Create the device id set
	hashtable_options_t devicesid_opts = {10000, 0.5f};
	hashtable_t* devicesid = hashtable_create(&string_eq, &string_hash, &devicesid_opts);
	coll_functor_t collect_devicedefid = {&functor_collect_devicedefid, devicesid};
	hashmap_foreach(patchers, &collect_devicedefid);
	coll_functor_t collect_deviceid = {&functor_collect_deviceid, devicesid};
	hashmap_foreach(repository->devices, &collect_deviceid);

	// Patch devices
	coll_functor_t patch_devices;
	functor_patch_devices_data_t patch_devices_data;
	patch_devices_data.patcheds = patcheds;
	patch_devices_data.patchings = repository->devices;
	patch_devices_data.patchers = patchers;
	patch_devices.data = &patch_devices_data;
	patch_devices.functor = &functor_patch_devices;

	hashtable_foreach(devicesid, &patch_devices);
	strcat(strcat(repository->version, "; "), patch_data.version);

	// Free old resources
	coll_unduper_t device_unduper;
	device_unduper.undupe = &device_undupe;
	hashmap_destroy(repository->devices, &device_unduper);

	coll_unduper_t unduper;
	coll_init_default_unduper(&unduper);
	hashtable_destroy(patch_data.devices, NULL);
	hashmap_destroy(patchers, &unduper);

	repository->devices = patcheds;
}

static device_t* create_device(hashmap_t* createds, const char* id, hashmap_t* sources) {

	assert(id!=NULL);
	assert(createds!=NULL);
	assert(sources!=NULL);

	if(string_eq(id, "root")) {
		return NULL;
	}

	device_t* device = hashmap_get(createds, id);
	if(device==NULL) {

		devicedef_t* source = hashmap_get(sources, id);

		// create device
		device = malloc(sizeof(device_t));
		device->id = source->id;
		device->user_agent = source->user_agent;
		device->capabilities = source->capabilities;
		device->parent = create_device(createds, source->fall_back, sources);

		// put device in the map
		hashmap_put(createds, id, device);
	}

	return device;
}

static device_t* patch_device(hashmap_t* patcheds, const char* id, hashmap_t* patchers, hashmap_t* patchings) {

	assert(id!=NULL);
	assert(patchers!=NULL);
	assert(patcheds!=NULL);
	assert(patchings!=NULL);

	if(string_eq(id, "root")) {
		return NULL;
	}

	device_t* patched = hashmap_get(patcheds, id);
	if(patched==NULL) {

		// create patched
		patched = malloc(sizeof(device_t));
		hashmap_options_t capabilities_opts = {100, 0.75f};
		patched->capabilities = hashmap_create(string_eq, string_hash, &capabilities_opts);

		devicedef_t* patcher = hashmap_get(patchers, id);
		device_t* patching = hashmap_get(patchings, id);

		if(patcher && patching) {

			patched->id = id;
			patched->user_agent = patching->user_agent;
			hashmap_putall(patched->capabilities, patching->capabilities);
			hashmap_putall(patched->capabilities, patcher->capabilities);

			// FIXME if fall_back==NULL error
			patched->parent = patch_device(patcheds, patcher->fall_back, patchers, patchings);
		}
		else if(patching) {

			patched->id = id;
			patched->user_agent = patching->user_agent;
			hashmap_putall(patched->capabilities, patching->capabilities);
			if(patching->parent!=NULL) {
				patched->parent = patch_device(patcheds, patching->parent->id, patchers, patchings);
			}
			else {
				patched->parent = NULL;
			}
		}
		else if(patcher){

			patched->id = id;
			patched->user_agent = patcher->user_agent;
			patched->capabilities = patcher->capabilities;

			patched->parent = patch_device(patcheds, patcher->fall_back, patchers, patchings);
		}
		else {
			// TODO error
			assert(false);
		}

		hashmap_put(patcheds, id, patched);
	}

	return patched;
}

void device_undupe(void* item, const void* data) {

	device_t* device = item;
	hashmap_destroy(device->capabilities, NULL);

	free(device);
}

const char* create_string(hashtable_t* strings, const char* string) {

	const char* exist = hashtable_get(strings, string);
	if(!exist) {
		// TODO check exist
		exist = malloc(sizeof(char) * (strlen(string) + 1));
		strcpy(exist, string);

		hashtable_add(strings, exist);
	}

	return exist;

}
