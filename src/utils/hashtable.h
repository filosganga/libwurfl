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

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "utils.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct _hashtable_t hashtable_t;

typedef struct {
	uint32_t initial_capacity;
	float load_factor;
} hashtable_options_t;


hashtable_t* hashtable_init(coll_equals_f* eq_fn, coll_hash_f* hash_fn, hashtable_options_t* options);

void hashtable_free(hashtable_t* hashtable, coll_unduper_f* unduper, void* unduper_data);


void* hashtable_get(hashtable_t* hashtable, const void* item);

void* hashtable_add(hashtable_t* hashtable, const void* item, coll_duper_f* duper, void* duper_data);

void* hashtable_remove(hashtable_t* hashtable, const void* item);

void hashtable_clear(hashtable_t* hashtable, coll_unduper_f* unduper, void* unduper_data);


bool hashtable_contains(hashtable_t* hashtable, void* item);

uint32_t hashtable_size(hashtable_t* hashtable);

bool hashtable_empty(hashtable_t* hashtable);

bool hashtable_foreach(hashtable_t* hashtable, coll_functor_f* functor, void* functor_data);

#endif /* HASHTABLE_H_ */
