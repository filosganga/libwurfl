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

typedef enum {ROOT, PATCH} resource_type_e;

typedef struct {
	char* version;
	hashtable_t* devices;
	resource_type_e type;
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

int resource_parse(resource_data_t* resource_data, const char* path, hashtable_t* strings);

const char* create_string(hashtable_t* strings, const char* string);

#endif /* PARSER_H_ */
