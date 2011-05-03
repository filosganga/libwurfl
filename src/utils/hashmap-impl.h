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
