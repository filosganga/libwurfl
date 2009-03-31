/*
 * parser.h
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "../repository.h"
#include "../collection/hashset.h"

typedef struct {
	char* version;
	hashset_t* devices;
} resource_data_t;

resource_data_t* parse_resource(char* resource);

#endif /* PARSER_H_ */
