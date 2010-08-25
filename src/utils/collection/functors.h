/*
 * functors.h
 *
 *  Created on: 24/ago/2010
 *      Author: filippodeluca
 */

#ifndef FUNCTORS_H_
#define FUNCTORS_H_

#include "collections.h"
#include "hashmap.h"

#include <stdint.h>

typedef struct {
	hashmap_t* map;
	void* (*key_get)(const void* item);
} functor_tomap_data_t;

int functor_tomap(const void* item, void* data);

typedef struct {
	uint32_t index;
	void** array;
} functor_toarray_data_t;

int functor_toarray(const void* item, void* data);

typedef struct {
	uint32_t nth;
	coll_predicate_t* predicate;
	void* found;
} functor_find_data_t;

int functor_find(const void* item, void* xtra);

void functor_init(coll_functor_f* function, void* data, coll_functor_t* functor);

#endif /* FUNCTORS_H_ */
