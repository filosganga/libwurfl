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

