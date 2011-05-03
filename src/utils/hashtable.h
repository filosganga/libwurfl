/* Copyright (C) 2011 Fantayeneh Asres Gizaw, Filippo De Luca
 *  
 * This file is part of libWURFL.
 *
 * libWURFL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or 
 * any later version.
 *
 * libWURFL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
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
