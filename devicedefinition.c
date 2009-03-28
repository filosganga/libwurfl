/*
 * devicedefinition.c
 *
 *  Created on: 28-mar-2009
 *      Author: filosganga
 */

#include "repository.h"
#include "collection/collections.h"
#include "collection/hashmap.h"

#include <stdlib.h>

typedef struct _devicedef_t {
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

char* devicedef_is_root(devicedef_t* devicedef) {
	return devicedef->actual_device_root;
}


