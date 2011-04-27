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

#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "utils.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct _hashmap_t hashmap_t;

typedef struct {
	uint32_t initial_capacity;
	float load_factor;
} hashmap_options_t;

hashmap_t* hashmap_init(coll_equals_f* key_equals, coll_hash_f* key_hash, hashmap_options_t* options);

void hashmap_free(hashmap_t* hashmap, coll_unduper_f* unduper, void* unduper_data);


void* hashmap_get(hashmap_t* hashmap, const void* key);

void* hashmap_put(hashmap_t* hashmap, const void* key, const void* item);

void hashmap_putall(hashmap_t* dest, const hashmap_t* src);

void* hashmap_remove(hashmap_t* hashmap, const void* key);

void hashmap_clear(hashmap_t* hashmap, coll_unduper_f* unduper, void* unduper_data);


uint32_t hashmap_size(hashmap_t* hashmap);

bool hashmap_empty(hashmap_t* hashmap);

bool hashmap_contains(hashmap_t* hashmap, const void* key);

bool hashmap_contains_item(hashmap_t* hashmap, const void* item);


bool hashmap_foreach(hashmap_t* hashmap, coll_functor_f* functor, void* functor_data);

//void* hashmap_find(hashmap_t* map, coll_predicate_f* predicate, void* predicate_data);

#endif /*HASHMAP_H_*/
