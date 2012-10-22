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

#ifndef FUNCTORS_H_
#define FUNCTORS_H_

#include "utils.h"
#include "hashtable.h"
#include "hashmap.h"
#include "patricia.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	hashmap_t* map;
	void* (*key_get)(const void* item);
} functor_tomap_data_t;

bool functor_tomap(const void* item, void* data);

typedef struct {
	uint32_t index;
	size_t size;
	void** array;
} functor_toarray_data_t;

bool functor_toarray(const void* item, void* data);

typedef struct {
	patricia_t* trie;
	void* (*key_get)(const void* item);
} functor_totrie_data_t;

bool functor_totrie(const void* item, void* data);

typedef struct {
	hashtable_t* set;
} functor_toset_data_t;

bool functor_toset(const void* item, void* data) ;

//typedef struct {
//	coll_predicate_t* predicate;
//	void* found;
//} functor_find_data_t;
//
//int functor_find(const void* item, void* xtra);

#endif /* FUNCTORS_H_ */
