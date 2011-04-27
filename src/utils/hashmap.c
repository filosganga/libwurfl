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

#include "hashmap-impl.h"

#include "hashtable-impl.h"
#include "functors.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <assert.h>

extern int errno;


// Item functions *********************************************************

bool item_eq(const void* litem, const void* ritem) {
	hashmap_item_t* lhashmap_item = (hashmap_item_t*)litem;
	hashmap_item_t* rhashmap_item = (hashmap_item_t*)ritem;

	return lhashmap_item->owner->key_equals(lhashmap_item->key, rhashmap_item->key);
}

hashmap_item_t* item_create(hashmap_t* hashmap, const void* key, const void* item) {

	hashmap_item_t* hashmap_item = malloc(sizeof(hashmap_item_t));
	hashmap_item->key = key;
	hashmap_item->item = item;
	hashmap_item->owner = hashmap;

	return hashmap_item;
}

typedef struct {
	coll_unduper_f* unduper;
	void* data;
} item_destroy_data_t;

void item_destroy(void* item, const void* data) {

	item_destroy_data_t* destroy_data = data;
	hashmap_item_t* hashmap_item = item;

	if(destroy_data && destroy_data->unduper) {
		destroy_data->unduper(hashmap_item->item, destroy_data->data);
	}

	free(hashmap_item);
}

uint32_t item_hash(const void *item) {
	const hashmap_item_t* hashmap_item = item;
	hashmap_t* map = hashmap_item->owner;

	return map->key_hash(hashmap_item->key);
}

//bool item_evaluate(const void* item, void* data) {
//	const hashmap_item_t* hashmap_item = item;
//	item_evaluate_data_t* ev_data = data;
//
//	return ev_data->key_predicate->evaluate(hashmap_item->key, ev_data->key_predicate->data);
//}



// Functors ***************************************************************

typedef struct {
	coll_functor_f* functor;
	void* data;

} item_functor_data_t;

bool item_functor(const void* item, void* data) {

	hashmap_item_t* hashmap_item = item;
	item_functor_data_t* functor_data = data;

	return functor_data->functor(hashmap_item->item, functor_data->data);
}

bool putall_functor(const void* item, void* data) {

	hashmap_item_t* hashmap_item = item;
	hashmap_t* dest = data;

	// FIXME add memory check
	hashmap_item_t* cloned = malloc(sizeof(hashmap_item_t));
	cloned->key = hashmap_item->key;
	cloned->item = hashmap_item->item;
	cloned->owner = dest;

	hashmap_item_t* replaced = hashtable_add(dest->hashtable, cloned);
	if(replaced!=NULL) {
		free(replaced);
	}

	return 0;
}

// Interface funcions *****************************************************

hashmap_t* hashmap_init(coll_equals_f* key_equals, coll_hash_f* key_hash, hashmap_options_t* options) {

	hashmap_t* map = malloc(sizeof(hashmap_t));

	map->key_hash = key_hash;
	map->key_equals = key_equals;

	hashtable_options_t hashtable_options;
	if(options!=NULL) {
		hashtable_options.initial_capacity = options->initial_capacity;
		hashtable_options.load_factor = options->load_factor;
	}
	else {
		hashtable_options.initial_capacity = 128;
		hashtable_options.load_factor = 1.5f;
	}

	map->hashtable = hashtable_init(&item_eq, &item_hash, &hashtable_options);

	return map;
}

void hashmap_free(hashmap_t* map, coll_unduper_f* unduper, void* unduper_data) {

	assert(map!=NULL);

	item_destroy_data_t item_destroy_data;
	item_destroy_data.unduper = unduper;
	item_destroy_data.data = unduper_data;

	hashtable_free(map->hashtable, &item_destroy, &item_destroy_data);
	free(map);
}

void* hashmap_put(hashmap_t* map, const void* key, const void* item) {

	// item can be NULL
	assert(map!=NULL);
	assert(key!=NULL);

	hashmap_item_t* hashmap_item = item_create(map, key, item);
	hashmap_item_t* replaced_item = hashtable_add(map->hashtable, hashmap_item);

	void *replaced = NULL;
	if(replaced_item!=NULL) {
		replaced = replaced_item->item;
		free(replaced_item);
	}

	return replaced;
}

void hashmap_putall(hashmap_t* dest, const hashmap_t* src) {

	assert(dest!=NULL);
	assert(src!=NULL);

	hashtable_foreach(src->hashtable, &putall_functor, dest);
}

void* hashmap_get(hashmap_t* map, const void* key) {

	assert(map!=NULL);
	assert(key!=NULL);

	void* item = NULL;

	hashmap_item_t map_item;
	map_item.key = key;
	map_item.owner = map;

	hashmap_item_t* found = hashtable_get(map->hashtable, &map_item);

	if(found){
		item = found->item;
	}

	return item;
}

void* hashmap_remove(hashmap_t* map, const void* key) {

	assert(map!=NULL);
	assert(key!=NULL);


	hashmap_item_t hashmap_item;
	hashmap_item.key = key;
	hashmap_item.owner = map;

	hashmap_item_t* removed_item = hashtable_remove(map->hashtable, &hashmap_item);

	void* removed = NULL;
	if(removed_item){
		removed = removed_item->item;
		free(removed_item);
	}

	return removed;
}

uint32_t hashmap_size(hashmap_t* map){

	assert(map!=NULL);

	return hashtable_size(map->hashtable);
}

bool hashmap_empty(hashmap_t* hashmap) {

	assert(hashmap!=NULL);

	return hashmap_size(hashmap)==0;
}

bool hashmap_contains(hashmap_t* hashmap, const void* key) {

	assert(hashmap!=NULL);

	return hashmap_get(hashmap, key) != NULL;
}

void hashmap_clear(hashmap_t* map, coll_unduper_f* unduper, void* unduper_data) {

	assert(map!=NULL);

	item_destroy_data_t item_destroy_data;
	item_destroy_data.unduper = unduper;
	item_destroy_data.data = unduper_data;

	hashtable_clear(map->hashtable, &item_destroy, &item_destroy_data);
}

//void* hashmap_find(hashmap_t* map, coll_predicate_t* predicate, void* predicate_data) {
//
//	assert(map!=NULL);
//	assert(predicate!=NULL);
//	assert(nth>=0);
//
//	void* found = NULL;
//
//	item_evaluate_data_t item_evaluate_data;
//	item_evaluate_data.key_predicate = predicate;
//
//	coll_predicate_t map_predicate;
//	map_predicate.evaluate = &item_evaluate;
//	map_predicate.data = &item_evaluate_data;
//
//	hashmap_item_t* map_item = hashtable_find(map->hashtable, &map_predicate, nth);
//	if(map_item != NULL) {
//		found = map_item->item;
//	}
//
//	return found;
//}

bool hashmap_foreach(hashmap_t* hashmap, coll_functor_f* functor, void* functor_data) {

	assert(hashmap!=NULL);
	assert(functor!=NULL);

	item_functor_data_t item_functor_data;
	item_functor_data.functor = functor;
	item_functor_data.data = functor_data;

	return hashtable_foreach(hashmap->hashtable, &item_functor, &item_functor_data);
}

