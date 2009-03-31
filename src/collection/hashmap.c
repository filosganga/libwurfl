#include "hashmap.h"
#include "hashlib.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct _hashmap_item_t {
	void *key;
	void* item;
	hashmap_t* owner;
} hashmap_item_t;

struct _hashmap_t {
	hshtbl* hashtable;
	coll_hash_f key_hash;
	coll_cmp_f key_equals;
};

static int item_cmp(void* litem, void* ritem) {
	hashmap_item_t* lhashmap_item = (hashmap_item_t*)litem;
	hashmap_item_t* rhashmap_item = (hashmap_item_t*)ritem;

	int equals = lhashmap_item->owner->key_equals(lhashmap_item->key, rhashmap_item->key);

	return equals==0;
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

	hashmap_item_t* hashmap_item = malloc(sizeof(hashmap_item_t));
	free(hashmap_item);
}

static unsigned long item_hash(void *item) {
	hashmap_item_t* hashmap_item = item;
	hashmap_t* map = hashmap_item->owner;

	return map->key_hash(hashmap_item->key);
}

static unsigned long item_rehash(void *item) {

	unsigned long hash = item_hash(item);
	return hash * 37;
}

hashmap_t* hashmap_create(coll_hash_f key_hash,coll_cmp_f key_equals) {

	hashmap_t* map = malloc(sizeof(hashmap_t));
	map->key_hash = key_hash;
	map->key_equals = key_equals;
	map->hashtable = hshinit(&item_hash, &item_rehash, &item_cmp, &item_clone, &item_free, 0);

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

u_int32_t hashmap_size(hashmap_t* hashmap){

	assert(hashmap!=NULL);

	return (u_int32_t)hshstatus(hashmap->hashtable).hentries;
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
	hashmap->hashtable = hshinit(&item_hash, &item_rehash, &item_cmp, &item_clone, &item_free, 0);
}

void hashmap_destroy(hashmap_t* hashmap) {

	hshkill(hashmap->hashtable);
	free(hashmap);
}



