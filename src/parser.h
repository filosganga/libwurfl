/*
 * parser.h
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "utils/hashmap.h"
#include "utils/hashtable.h"

typedef enum {ROOT, PATCH} resource_type_e;

typedef struct {
	char* version;
	hashmap_t* devices;
	resource_type_e type;
} resource_data_t;

int parser_parse(const char* path, hashtable_t* strings, resource_data_t* resource_data);

#endif /* PARSER_H_ */
