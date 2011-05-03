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
 * along with libWURFL.  If not, see <http://www.gnu.org/licenses/>.
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

bool hashmap_contains_value(hashmap_t* hashmap, const void* item);


bool hashmap_foreach(hashmap_t* hashmap, coll_functor_f* functor, void* functor_data);

bool hashmap_foreach_value(hashmap_t* hashmap, coll_functor_f* functor, void* functor_data);

//void* hashmap_find(hashmap_t* map, coll_predicate_f* predicate, void* predicate_data);

#endif /*HASHMAP_H_*/
