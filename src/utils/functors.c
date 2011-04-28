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

bool functor_tomap(const void* item, void* data) {
	functor_tomap_data_t* tomap_data = data;

	void* key = tomap_data->key_get(item);
	hashmap_put(tomap_data->map, key, item);

	return false;
}

bool functor_toarray(const void* item, void* data) {
	functor_toarray_data_t* tadata = data;

	if(tadata->index < tadata->size) {
		tadata->array[tadata->index++] = (void*)item;
	}

	return tadata->index >= tadata->size;
}

bool functor_totrie(const void* item, void* data) {
	functor_totrie_data_t* functor_data = (functor_totrie_data_t*)data;

	patricia_put(functor_data->trie, functor_data->key_get(item), item);

	return false;
}

bool functor_toset(const void* item, void* data) {
	functor_toset_data_t* functor_data = (functor_toset_data_t*)data;

	hashtable_add(functor_data->set, item, NULL, NULL);

	return false;
}

