/*
 * hashtable.h
 *
 *  Created on: May 17, 2010
 *      Author: filosganga
 */
#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "collections.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct _hashtable_t hashtable_t;

typedef struct {
	uint32_t initial_capacity;
	float load_factor;
} hashtable_options_t;


hashtable_t* hashtable_create(coll_equals_f eq_fn, coll_hash_f hash_fn, hashtable_options_t* options);

void hashtable_destroy(hashtable_t* hashtable, coll_unduper_t* unduper);


void* hashtable_get(hashtable_t* hashtable, const void* item);

void* hashtable_add(hashtable_t* hashtable, const void* item);

void* hashtable_remove(hashtable_t* hashtable, const void* item);

void hashtable_clear(hashtable_t* hashtable, coll_unduper_t* unduper);


bool hashtable_contains(hashtable_t* hashtable, void* item);

uint32_t hashtable_size(hashtable_t* hashtable);

bool hashtable_empty(hashtable_t* hashtable);

bool hashtable_foreach(hashtable_t* hashtable, coll_functor_t* functor);

void* hashtable_find(hashtable_t* hashtable, coll_predicate_t* predicate, uint32_t nth);

#endif /* HASHTABLE_H_ */
