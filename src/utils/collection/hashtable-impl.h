/*
 * hashtable-impl.h
 *
 *  Created on: May 17, 2010
 *      Author: filosganga
 */

#ifndef HASHTABLEIMPL_H_
#define HASHTABLEIMPL_H_

#include "hashtable.h"

#include "utils/memory/allocator.h"

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
	coll_hash_f hash_fn;
	coll_equals_f eq_fn;
	uint32_t threshold;
	uint32_t size;
	float lfactor;
	allocator_t* allocator;
};

uint32_t hashtable_index(const int32_t hash, uint32_t length);

hashtable_entry_t* hashtable_find_entry(hashtable_t* hashtable, uint32_t table_index, const void* item);
hashtable_entry_t* hashtable_add_entry(hashtable_t* hashtable, uint32_t table_index, const void* item);
hashtable_entry_t* hashtable_remove_entry(hashtable_t* hashtable, uint32_t table_index, const void* item);

#endif /* HASHTABLEIMPL_H_ */
