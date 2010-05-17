/*
 * hashtable-impl.h
 *
 *  Created on: May 17, 2010
 *      Author: filosganga
 */

#ifndef HASHTABLEIMPL_H_
#define HASHTABLEIMPL_H_

#include "hashtable.h"

/**
 * The default initial capacity - MUST be a power of two.
 */
#define DEFAULT_INITIAL_CAPACITY 16

/**
 * The maximum capacity, used if a higher value is implicitly specified
 * by either of the constructors with arguments.
 * MUST be a power of two <= 1<<30.
 */
#define MAXIMUM_CAPACITY (1 << 30)

/**
 * The load factor used when none specified in constructor.
 */
#define DEFAULT_LOAD_FACTOR 0.75f

typedef struct _hashtable_entry_t hashtable_entry_t;

struct _hashtable_entry_t {
	hashtable_entry_t* next;
	int32_t* hash;
	void* item;
};

struct _hashtable_t {
	hashtable_entry_t** table;
	uint32_t table_size;
	coll_hash_f hash_fn;
	coll_equals_f eq_fn;
	uint32_t threshold;
	uint32_t size;
	float lfactor;
};

uint32_t index(const int32_t hash, uint32_t length);

hashtable_entry_t* find(hashtable_t* hashtable, uint32_t table_index, const void* item);
hashtable_entry_t* add(hashtable_t* hashtable, uint32_t table_index, const void* item);
hashtable_entry_t* remove(hashtable_t* hashtable, uint32_t table_index, const void* item);

#endif /* HASHTABLEIMPL_H_ */
