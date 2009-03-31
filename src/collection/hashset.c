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

typedef struct _to_array_data_t {
	void** array;
	int index;
} to_array_data_t;

static int to_array(void* item, void* data, void* xtra) {

	hashset_item_t* hashset_item = (hashset_item_t*)item;
	to_array_data_t* to_array_data = data;

	to_array_data->array[to_array_data->index] = hashset_item->item;
	to_array_data->index++;

	return 0;
}

static int exec_functor(void* item, void* data, void* xtra) {

	hashset_item_t* hashset_item = (hashset_item_t*)item;
	functor_data_t* functor_data = data;

	return functor_data->functor(hashset_item->item,functor_data->data);
}


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

	to_array_data_t to_array_data;
	to_array_data.array = malloc(sizeof(void*) * hashset_size(hashset));
	to_array_data.index = 0;

	hshwalk(hashset->hashtable, &to_array, &to_array_data);

	return to_array_data.array;
}

int hashset_foreach(hashset_t* hashset, coll_functor_f functor, void* context) {

	assert(hashset!=NULL);
	assert(functor!=NULL);

	functor_data_t functor_data;
	functor_data.data = context;
	functor_data.functor = functor;

	return hshwalk(hashset->hashtable, &exec_functor, &functor_data);
}
