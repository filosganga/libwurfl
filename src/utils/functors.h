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

#ifndef FUNCTORS_H_
#define FUNCTORS_H_

#include "utils.h"
#include "hashmap.h"
#include "patricia.h"

#include <stdint.h>

typedef struct {
	hashmap_t* map;
	void* (*key_get)(const void* item);
} functor_tomap_data_t;

int functor_tomap(const void* item, void* data);

typedef struct {
	uint32_t index;
	size_t size;
	void** array;
} functor_toarray_data_t;

int functor_toarray(const void* item, void* data);

typedef struct {
	trie_t* trie;
	void* (*key_get)(const void* item);
} functor_totrie_data_t;

int functor_totrie(const void* item, void* data);

//typedef struct {
//	coll_predicate_t* predicate;
//	void* found;
//} functor_find_data_t;
//
//int functor_find(const void* item, void* xtra);

#endif /* FUNCTORS_H_ */
