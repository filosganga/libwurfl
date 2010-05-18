/*
 * devidedef-impl.h
 *
 *  Created on: 31-mar-2009
 *      Author: filosganga
 */

#ifndef DEVICEDEFIMPL_H_
#define DEVICEDEFIMPL_H_

#include "devicedef.h"
#include "utils/collection/hashmap.h"
#include "utils/collection/hashtable.h"

struct _devicedef_t {
	char* id;
	char* user_agent;
	char* fall_back;
	int actual_device_root;
	hashmap_t* capabilities;
};

struct _devicedefs_t {
	hashmap_t* devices;
};


devicedef_t* devicedef_create(char* id, char* user_agent, char* fall_back, int actual_device_root, hashmap_t* capabilities);

devicedefs_t* devicedefs_create_frommap(hashmap_t* devices);

devicedefs_t* devicedefs_create_fromset(hashtable_t* devices);

devicedefs_t* devicedefs_create_fromarray(devicedef_t** devices);

#endif /* DEVICEDEFIMPL_H_ */
