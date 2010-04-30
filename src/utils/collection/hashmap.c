#include "hashmap.h"
#include "utils/hashlib/hashlib.h"
#include "gnulib/error.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <assert.h>

extern int errno;

typedef struct _hashmap_item_t {
	void *key;
	void* item;
	hashmap_t* owner;
} hashmap_item_t;

struct _hashmap_t {
	hshtbl* hashtable;
	coll_hash_f key_hash;
	coll_hash_f key_rehash;
	coll_equals_f key_equals;
};

// Item functions *********************************************************

static bool item_eq(const void* litem, const void* ritem) {
	hashmap_item_t* lhashmap_item = (hashmap_item_t*)litem;
	hashmap_item_t* rhashmap_item = (hashmap_item_t*)ritem;

	return lhashmap_item->owner->key_equals(lhashmap_item->key, rhashmap_item->key);
}

static hashmap_item_t* item_create(hashmap_t* hashmap, void* key, void* item) {
	hashmap_item_t* hashmap_item = malloc(sizeof(hashmap_item_t));
	hashmap_item->key = key;
	hashmap_item->item = item;
	hashmap_item->owner = hashmap;

	return hashmap_item;
}

static void* item_clone(void *item) {

	return item;
}

static void item_free(void *item) {

	free(item);
}

static unsigned long item_hash(void *item) {
	hashmap_item_t* hashmap_item = item;
	hashmap_t* map = hashmap_item->owner;

	return map->key_hash(hashmap_item->key);
}

static unsigned long item_rehash(void *item) {

	hashmap_item_t* hashmap_item = item;
	hashmap_t* map = hashmap_item->owner;

	return map->key_rehash(hashmap_item->key);
}

// Walker functions *******************************************************

typedef struct {
	void** array;
	int index;
} walk_toarray_data_t;

static int walk_toarray(void* item, void* data, void* xtra) {

	hashmap_item_t* hashmap_item = item;
	walk_toarray_data_t* to_array_data = data;

	to_array_data->array[to_array_data->index] = hashmap_item->item;
	to_array_data->index++;

	return 0;
}

typedef struct {
	void* found;
	coll_predicate_t* predicate;
} walk_find_data_t;

static int walk_find(void* item, void* data, void* xtra) {

	hashmap_item_t* hashmap_item = item;
	walk_find_data_t* walker_data = data;

	coll_predicate_f predicate = walker_data->predicate->predicate;
	void* predicate_data = walker_data->predicate->data;

	if(predicate(hashmap_item->item, predicate_data)){
		walker_data->found = hashmap_item->item;
		return 1;
	}

	return 0;
}

typedef struct {
	hashmap_t* target;
	coll_predicate_t* predicate;
} walk_select_data_t;

static int walk_select(void* item, void* data, void* xtra) {

	hashmap_item_t* hashmap_item = item;
	walk_select_data_t* walker_data = (walk_select_data_t*)data;

	coll_predicate_f predicate = walker_data->predicate->predicate;
	void* predicate_data = walker_data->predicate->data;

	if(predicate(hashmap_item->item, predicate_data)) {
		hashmap_put(walker_data->target, hashmap_item->key, hashmap_item->item);
		// FIXME check status
	}

	return 0;
}

static int walk_functor(void* item, void* data, void* xtra) {

	hashmap_item_t* hashmap_item = item;
	coll_functor_t* functor = data;

	return functor->functor(hashmap_item->item, functor->data);
}

// Interface funcions *****************************************************

hashmap_t* hashmap_create(coll_hash_f key_hash, coll_hash_f key_rehash, coll_equals_f key_equals) {

	hashmap_t* map = malloc(sizeof(hashmap_t));
	if(map==NULL) {
		error(1, errno, "Error allocating map");
	}

	map->key_hash = key_hash;
	map->key_rehash = key_rehash;
	map->key_equals = key_equals;
	map->hashtable = hshinit(&item_hash, &item_rehash, &item_eq, &item_clone, &item_free, 0);

	return map;
}


void* hashmap_put(hashmap_t* hashmap, void* key, void* item) {

	// item can be NULL
	assert(hashmap!=NULL);
	assert(key!=NULL);

	hashmap_item_t* hashmap_item = item_create(hashmap, key, item);
	hshinsert(hashmap->hashtable, hashmap_item);

	return item;
}

void* hashmap_get(hashmap_t* hashmap, void* key) {

	assert(hashmap!=NULL);
	assert(key!=NULL);

	void* item = NULL;

	hashmap_item_t hashMapItem;
	hashMapItem.key = key;
	hashMapItem.owner = hashmap;

	hashmap_item_t* found = hshfind(hashmap->hashtable, &hashMapItem);

	if(found){
		item = found->item;
		free(found);
	}

	return item;
}

void* hashmap_remove(hashmap_t* hashmap, void* key) {

	assert(hashmap!=NULL);
	assert(key!=NULL);

	void* item = NULL;

	hashmap_item_t hashMapItem;
	hashMapItem.key = key;
	hashMapItem.owner = hashmap;

	hashmap_item_t* removed = hshdelete(hashmap->hashtable, &hashMapItem);

	if(removed){
		item = removed->item;
	}

	return item;
}

uint32_t hashmap_size(hashmap_t* hashmap){

	assert(hashmap!=NULL);

	return (uint32_t)hshstatus(hashmap->hashtable).hentries;
}

int hashmap_empty(hashmap_t* hashmap) {

	assert(hashmap!=NULL);

	return hashmap_size(hashmap)==0;
}

int hashmap_contain_key(hashmap_t* hashmap, void* key) {

	assert(hashmap!=NULL);

	return hashmap_get(hashmap, key) != NULL;
}

void hashmap_clear(hashmap_t* hashmap) {

	assert(hashmap!=NULL);

	hshkill(hashmap->hashtable);
	hashmap->hashtable = hshinit(&item_hash, &item_rehash, &item_eq, &item_clone, &item_free, 0);
}

void hashmap_destroy(hashmap_t* hashmap) {

	hshkill(hashmap->hashtable);
	free(hashmap);
}

void hashmap_to_array(hashmap_t* hashmap, void** array) {

	assert(hashmap!=NULL);

	walk_toarray_data_t to_array_data;
	to_array_data.array = array;
	to_array_data.index = 0;

	hshwalk(hashmap->hashtable, &walk_toarray, &to_array_data);
}

void* hashmap_find(hashmap_t* hashmap, coll_predicate_t* predicate) {

	assert(hashmap!=NULL);
	assert(predicate!=NULL);

	walk_find_data_t walker_data;
	walker_data.found=NULL;
	walker_data.predicate = predicate;

	hshwalk(hashmap->hashtable, &walk_find, &walker_data);

	return walker_data.found;
}

int hashmap_foreach(hashmap_t* hashmap, coll_functor_t* functor) {

	assert(hashmap!=NULL);
	assert(functor!=NULL);

	return hshwalk(hashmap->hashtable, &walk_functor, functor);
}

hashmap_t* hashmap_select(hashmap_t* hashmap, coll_predicate_t* predicate) {

	assert(hashmap!=NULL);
	assert(predicate!=NULL);

	walk_select_data_t walker_data;
	walker_data.target =  hashmap_create(hashmap->key_hash, hashmap->key_rehash, hashmap->key_equals);
	walker_data.predicate = predicate;

	hshwalk(hashmap->hashtable, &walk_select, &walker_data);

	return walker_data.target;
}

