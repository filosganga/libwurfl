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

#ifndef HASHTABLEIMPL_H_
#define HASHTABLEIMPL_H_

#include "hashtable.h"

/**
 * The default initial capacity - MUST be a power of two.
 */
#define HASHTABLE_DEFAULT_INIT_CAPACITY 16

/**
 * The maximum capacity, used if a higher value is implicitly specified
 * by either of the constructors with arguments.
 * MUST be a power of two <= 1<<30.
 */
#define MAXIMUM_CAPACITY (1 << 30)

/**
 * The load factor used when none specified in constructor.
 */
#define HASHTABLE_DEFAULT_LFACTOR 0.75f

typedef struct _hashtable_entry_t hashtable_entry_t;

struct _hashtable_entry_t {
	hashtable_entry_t* next;
	int32_t hash;
	void* item;
};

struct _hashtable_t {
	hashtable_entry_t** table;
	uint32_t table_size;
	uint32_t table_used;
	coll_hash_f* hash_fn;
	coll_equals_f* eq_fn;
	uint32_t threshold;
	uint32_t size;
	float lfactor;
};

uint32_t hashtable_index(const int32_t hash, uint32_t length);

hashtable_entry_t* hashtable_find_entry(hashtable_t* hashtable, uint32_t table_index, const void* item);
hashtable_entry_t* hashtable_add_entry(hashtable_t* hashtable, uint32_t table_index, const void* item);
hashtable_entry_t* hashtable_remove_entry(hashtable_t* hashtable, uint32_t table_index, const void* item);

#endif /* HASHTABLEIMPL_H_ */
