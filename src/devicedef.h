/*
 * devicedef.h
 *
 *  Created on: 19/apr/2011
 *      Author: filippodeluca
 */

#ifndef DEVICEDEF_H_
#define DEVICEDEF_H_

#include "utils/hashmap.h"

typedef struct {
	const char* id;
	const char* user_agent;
	const char* fall_back;
	bool actual_device_root;
	hashmap_t* capabilities;
} devicedef_t;

int devicedef_cmp(const void* left, const void* right);

uint32_t devicedef_hash(const void* item);

bool devicedef_eq(const void* litem, const void* ritem);

#endif /* DEVICEDEF_H_ */
