/*
 * functors.c
 *
 *  Created on: 24/ago/2010
 *      Author: filippodeluca
 */

#include "functors.h"

int functor_tomap(const void* item, void* data) {
	functor_tomap_data_t* tomap_data = data;

	void* key = tomap_data->key_get(item);
	hashmap_put(tomap_data->map, key, item);

	return 0;
}

int functor_toarray(const void* item, void* data) {
	functor_toarray_data_t* toarray_data = data;

	toarray_data->array[toarray_data->index++] = item;

	return 0;
}

//int functor_find(const void* item, void* xtra) {
//
//	int found = 0;
//
//	functor_find_data_t* data = xtra;
//	if(data->predicate->evaluate(item, data->predicate->data)) {
//		data->found = item;
//		found = 1;
//	}
//
//	return found;
//}
