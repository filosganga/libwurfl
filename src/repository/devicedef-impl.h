/*
 * devidedef-impl.h
 *
 *  Created on: 31-mar-2009
 *      Author: filosganga
 */

#ifndef DEVIDEDEFIMPL_H_
#define DEVIDEDEFIMPL_H_

#include "devicedef.h"
#include "utils/collection/hashmap.h"
#include "utils/collection/collections.h"

struct _devicedef_t {
	char* id;
	char* user_agent;
	char* fall_back;
	int actual_device_root;
	hashmap_t* capabilities;
};

devicedef_t* devicedef_create(char* id, char* user_agent, char* fall_back, int actual_device_root, hashmap_t* capabilities);

#endif /* DEVIDEDEFIMPL_H_ */
