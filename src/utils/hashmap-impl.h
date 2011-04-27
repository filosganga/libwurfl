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

#ifndef HASHMAP_IMPL_H_
#define HASHMAP_IMPL_H_

#include "hashmap.h"
#include "hashtable-impl.h"

#include <stdbool.h>

typedef struct _hashmap_item_t {
	const void *key;
	void* item;
	hashmap_t* owner;
} hashmap_item_t;

struct _hashmap_t {
	hashtable_t* hashtable;
	coll_hash_f* key_hash;
	coll_equals_f* key_equals;
};

// Item *******************************************************************

hashmap_item_t* item_init(hashmap_t* hashmap, const void* key, const void* item);

void item_free(void* item, const void* xtra);

bool item_eq(const void* litem, const void* ritem);

uint32_t item_hash(const void *item);

bool item_functor(const void* item, void* data);

// Functors ***************************************************************

//typedef struct {
//	coll_predicate_t* key_predicate;
//} item_evaluate_data_t;
//
//bool item_evaluate(const void* item, void* data);

typedef struct {
	hashmap_t* dest;
} putall_data_t;

bool putall_functor(const void* item, void* data);

#endif /* HASHMAP_IMPL_H_ */
