/*
 * hashmap-impl.h
 *
 *  Created on: 19/apr/2011
 *      Author: filippodeluca
 */

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
	coll_hash_f key_hash;
	coll_equals_f key_equals;
};

// Item *******************************************************************

hashmap_item_t* item_create(hashmap_t* hashmap, const void* key, const void* item);

void item_destroy(void* item, const void* xtra);

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
