/*
 * devicedefinition.c
 *
 *  Created on: 28-mar-2009
 *      Author: filosganga
 */

#include "devicedef-impl.h"

#include <stdlib.h>
#include <string.h>

struct _devicedef_t {
	char* id;
	char* user_agent;
	char* fall_back;
	int actual_device_root;
	hashmap_t* capabilities;
	hashmap_t* groups;
};

static devicedef_t* devicedef_alloc() {

	devicedef_t* devicedef = malloc(sizeof(devicedef_t));
	return devicedef;
}

devicedef_t* devicedef_create(char* id, char* user_agent, char* fall_back, int actual_device_root, hashmap_t* capabilities, hashmap_t* groups) {

	devicedef_t* devicedef = devicedef_alloc();
	devicedef->id = id;
	devicedef->user_agent = user_agent;
	devicedef->fall_back = fall_back;
	devicedef->actual_device_root = actual_device_root;
	devicedef->capabilities = capabilities;
	devicedef->groups = groups;

	return devicedef;

}

void devicedef_destroy(devicedef_t* devicedef) {

	free(devicedef->id);
	free(devicedef->user_agent);
	free(devicedef->fall_back);
	hashmap_destroy(devicedef->capabilities);
	hashmap_destroy(devicedef->groups);
}

char* devicedef_get_id(devicedef_t* devicedef) {
	return devicedef->id;
}

char* devicedef_get_user_agent(devicedef_t* devicedef) {
	return devicedef->user_agent;
}

char* devicedef_get_fall_back(devicedef_t* devicedef) {
	return devicedef->fall_back;
}

int devicedef_is_root(devicedef_t* devicedef) {
	return devicedef->actual_device_root;
}

int devicedef_cmp(const void* litem, const void* ritem) {

	devicedef_t* ldevicedef = (devicedef_t*)litem;
	devicedef_t* rdevicedef = (devicedef_t*)ritem;

	return strcmp(ldevicedef->id, rdevicedef->id);
}

int devicedef_equals(const void* litem, const void* ritem) {

	return devicedef_cmp(litem, ritem) == 0;
}

unsigned long devicedef_hash(const void* item) {

	devicedef_t* devicedef = (devicedef_t*)item;

	return string_hash(devicedef->id);
}

unsigned long devicedef_rehash(const void* item) {

	devicedef_t* devicedef = (devicedef_t*)item;

	return string_rehash(devicedef->id);
}
