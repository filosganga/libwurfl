/*
 * parser.h
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "utils/memory/allocator.h"

typedef struct _resource_data_t resource_data_t;

resource_data_t resource_parse(const char* path);

#endif /* PARSER_H_ */
