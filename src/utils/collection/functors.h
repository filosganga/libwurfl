/*
 * functors.h
 *
 *  Created on: 24/ago/2010
 *      Author: filippodeluca
 */

#ifndef FUNCTORS_H_
#define FUNCTORS_H_

#include "collections.h"

typedef struct {
	hashmap_t* map;
	void* (*key_get)(const void* item);
} tomap_data_t;

int tomap_functor(const void* item, void* data) {
	tomap_data_t* tomap_data = data;

	void* key = tomap_data->key_get(item);
	hashmap_put(tomap_data->map, key, item);

	return 0;
}

#endif /* FUNCTORS_H_ */
