/*
 * parser.h
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "utils/hashmap.h"
#include "utils/hashtable.h"

typedef enum {ROOT, PATCH} resource_type_e;

typedef struct {
	char* version;
	hashmap_t* devices;
	resource_type_e type;
} resource_data_t;

int resource_parse(resource_data_t* resource_data, const char* path, hashtable_t* strings);

#endif /* RESOURCE_H_ */
