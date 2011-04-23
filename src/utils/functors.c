/*
 * Copyright 2011 ff-dev.org
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Written by Filippo De Luca <me@filippodeluca.com>.  */

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

int functor_totrie(const void* item, void* data) {
	functor_totrie_data_t* functor_data = (functor_totrie_data_t*)data;

	trie_put(functor_data->trie, functor_data->key_get(item), item);

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
