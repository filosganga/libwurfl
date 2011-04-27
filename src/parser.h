/*
 * Copyright 2011 ff-dev.org
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Written by Filippo De Luca <me@filippodeluca.com>.  */

#ifndef PARSER_H_
#define PARSER_H_

#include "utils/hashmap.h"
#include "utils/hashtable.h"

typedef struct {
	char* version;
	hashmap_t* devices;
} parser_data_t;

int parser_parse(const char* path, parser_data_t* resource_data);

#endif /* PARSER_H_ */
