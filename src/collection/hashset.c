/*
 * hashset.c
 *
 *  Created on: 29-mar-2009
 *      Author: filosganga
 */

#include "hashset.h"
#include "hashlib.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

struct _hashset_t {
	hshtbl* hashtable;
	coll_hash_f item_hash, item_rehash;
	coll_equals_f item_equals;
};

typedef struct _hashset_item_t {
	void* item;
	hashset_t* owner;
} hashset_item_t;

// Item functions *********************************************************

static int hashset_item_cmp(void* litem, void* ritem) {
	hashset_item_t* lhashset_item = (hashset_item_t*)litem;
	hashset_item_t* rhashset_item = (hashset_item_t*)ritem;

	int equals = lhashset_item->owner->item_equals(lhashset_item->item, rhashset_item->item);

	return !equals;
}

static hashset_item_t* hashset_item_create(hashset_t* hashset, void* item) {
	hashset_item_t* created = malloc(sizeof(hashset_item_t));
	created->item = item;
	created->owner = hashset;

	return created;
}

static void* hashset_item_clone(void *item) {

	return item;
}

static void hashset_item_free(void *item) {

	hashset_item_t* hashset_item = (hashset_item_t*)item;
	free(hashset_item);
}

static unsigned long hashset_item_hash(void *item) {
	hashset_item_t* hashset_item = (hashset_item_t*)item;
	return hashset_item->owner->item_hash(hashset_item->item);
}

static unsigned long hashset_item_rehash(void *item) {

	hashset_item_t* hashset_item = (hashset_item_t*)item;

	return hashset_item->owner->item_rehash(hashset_item->item);
}

// Walker functions *******************************************************

typedef struct {
	void** array;
	int index;
} walk_toarray_data_t;

static int walk_toarray(void* item, void* data, void* xtra) {

	hashset_item_t* hashset_item = (hashset_item_t*)item;
	walk_toarray_data_t* to_array_data = data;

	to_array_data->array[to_array_data->index] = hashset_item->item;
	to_array_data->index++;

	return 0;
}

typedef struct {
	void* found;
	coll_predicate_t* predicate;
} walk_find_data_t;

static int walk_find(void* item, void* data, void* xtra) {

	hashset_item_t* hashset_item = (hashset_item_t*)item;
	walk_find_data_t* walker_data = (walk_find_data_t*)data;

	coll_predicate_f predicate = walker_data->predicate->predicate;
	void* predicate_data = walker_data->predicate->data;

	if(predicate(hashset_item->item, predicate_data)){
		find_walker_data->found = hashset_item->item;
		return 1;
	}

	return 0;
}

typedef struct {
	coll_predicate_t* predicate;
} walk_filter_data_t;

static int walk_filter(void* item, void* data, void* xtra) {

	hashset_item_t* hashset_item = (hashset_item_t*)item;
	walk_filter_data_t* walker_data = (walk_filter_data_t*)data;

	coll_predicate_f predicate = walker_data->predicate->predicate;
	void* predicate_data = walker_data->predicate->data;

	if(!predicate(hashset_item->item, predicate_data)){
		hashset_remove(hashset_item->owner, hashset_item->item);
		// TODO how destroy the removed elements
	}

	return 0;
}

typedef struct {
	hashset_t* target;
	coll_predicate_t* predicate;
} walk_select_data_t;

static int walk_select(void* item, void* data, void* xtra) {

	hashset_item_t* hashset_item = (hashset_item_t*)item;
	walk_select_data_t* walker_data = (walk_select_data_t*)data;

	coll_predicate_f predicate = walker_data->predicate->predicate;
	void* predicate_data = walker_data->predicate->data;

	if(predicate(hashset_item->item, predicate_data)) {
		hashset_add(walker_data->target, hashset_item->item);
		// FIXME check status
	}

	return 0;
}

static int exec_functor(void* item, void* data, void* xtra) {

	hashset_item_t* hashset_item = (hashset_item_t*)item;
	coll_functor_t* functor = data;

	return functor->functor(hashset_item->item, functor->data);
}

// Hashset implementation *************************************************

hashset_t* hashset_create(coll_hash_f item_hash, coll_hash_f item_rehash, coll_equals_f item_equals) {

	hashset_t* hashset = malloc(sizeof(hashset_t));

	hashset->item_equals = item_equals;
	hashset->item_hash = item_hash;
	hashset->item_rehash = item_rehash;

	hashset->hashtable = hshinit(&hashset_item_hash, &hashset_item_rehash, &hashset_item_cmp, &hashset_item_clone, &hashset_item_free, 0);

	return hashset;
}

void* hashset_add(hashset_t* hashset, void* item) {

	assert(hashset!=NULL);
	assert(item!=NULL);

	hashset_item_t* hashset_item = hashset_item_create(hashset, item);
	hshinsert(hashset->hashtable, hashset_item);

	if(hshstatus(hashset->hashtable).herror!=hshOK){
		// TODO error
	}


	return item;
}

void* hashset_get(hashset_t* hashset, void* item) {

	assert(hashset!=NULL);
	assert(item!=NULL);

	void* found_item = NULL;

	hashset_item_t hashset_item;
	hashset_item.item = item;
	hashset_item.owner = hashset;

	hashset_item_t* found = hshfind(hashset->hashtable, &hashset_item);

	if(found) {
		found_item = found->item;
	}

	return found_item;
}

void* hashset_remove(hashset_t* hashset, void* item) {

	assert(hashset!=NULL);
	assert(item!=NULL);

	void* removed_item = NULL;

	hashset_item_t hashset_item;
	hashset_item.item = item;
	hashset_item.owner = hashset;


	hashset_item_t* removed = hshdelete(hashset->hashtable, &hashset_item);

	if(removed) {
		removed_item = removed->item;
		free(removed);
	}

	return removed_item;
}

u_int32_t hashset_size(hashset_t* hashset) {

	assert(hashset!=NULL);

	return (u_int32_t)hshstatus(hashset->hashtable).hentries;
}

int hashset_empty(hashset_t* hashset) {

	assert(hashset!=NULL);

	return hashset_size(hashset)==0;
}

int hashset_contains(hashset_t* hashset, void* item){

	assert(hashset!=NULL);

	return hashset_get(hashset, item) != NULL;

}

void hashset_clear(hashset_t* hashset){

	assert(hashset!=NULL);

	hshkill(hashset->hashtable);
	hashset->hashtable = hshinit(&hashset_item_hash, &hashset_item_rehash, &hashset_item_cmp, &hashset_item_clone, &hashset_item_free, 0);
}

void hashset_destroy(hashset_t* hashset){

	assert(hashset!=NULL);

	hshkill(hashset->hashtable);
	free(hashset);
}

void** hashset_to_array(hashset_t* hashset) {

	assert(hashset!=NULL);

	walk_toarray_data_t to_array_data;
	to_array_data.array = malloc(sizeof(void*) * hashset_size(hashset));
	to_array_data.index = 0;

	hshwalk(hashset->hashtable, &walk_toarray, &to_array_data);

	return to_array_data.array;
}

int hashset_foreach(hashset_t* hashset, coll_functor_t functor){

	assert(hashset!=NULL);
	assert(functor!=NULL);

	return hshwalk(hashset->hashtable, &exec_functor, functor);
}

void* hashset_find(hashset_t* hashset, coll_predicate_t predicate) {

	assert(hashset!=NULL);
	assert(predicate!=NULL);

	walk_find_data_t find_walker_data;
	find_walker_data.found = NULL;
	find_walker_data.predicate = predicate;

	hshwalk(hashset->hashtable, &walk_find, &find_walker_data);

	return find_walker_data.found;
}

hashset_t* hashset_select(hashset_t* hashset, coll_predicate_t predicate) {

	assert(hashset!=NULL);
	assert(predicate!=NULL);

	walk_select_data_t walker_data;
	walker_data.target = hashset_create(hashset->item_hash, hashset->item_rehash, hashset->item_equals);
	walker_data.predicate = predicate;

	hshwalk(hashset->hashtable, &walk_select, &walker_data);

	return walker_data.target;
}

void hashset_filter(hashset_t* hashset, coll_predicate_t predicate) {

	assert(hashset!=NULL);
	assert(predicate!=NULL);

	walk_filter_data_t walker_data;
	walker_data.predicate = predicate;

	hshwalk(hashset->hashtable, walk_filter, &walker_data);
}
