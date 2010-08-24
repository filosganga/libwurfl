/*
 * parser.h
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "utils/collection/hashtable.h"
#include "utils/collection/hashmap.h"
#include "utils/collection/collections.h"

#include <stdlib.h>

typedef struct {
	char* version;
	hashtable_t* devices;
} resource_data_t;

typedef struct {
	const char* id;
	const char* user_agent;
	const char* fall_back;
	bool actual_device_root;
	hashmap_t* capabilities;
} devicedef_t;

int devicedef_cmp(const void* ldevicedef, const void* rdevicedef);

uint32_t devicedef_hash(const void* item);

bool devicedef_eq(const void* litem, const void* ritem);

const char* devicedef_id(const devicedef_t* devicedef);

int resource_parse(const char* path, resource_data_t* resource_data);

#endif /* PARSER_H_ */
