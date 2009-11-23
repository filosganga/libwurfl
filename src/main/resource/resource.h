/*
 * parser.h
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "../collection/hashset.h"

typedef struct _resource_t resource_t;

typedef struct _resource_data_t resource_data_t;

resource_data_t* resource_parse(resource_t* resource);

resource_t* resource_create(char* path);

void resource_destroy(resource_t* resource);

void resource_data_destroy(resource_data_t* resource_data);

#endif /* PARSER_H_ */
