#include "hashmap.h"
#include "hashlib.h"

#include <stdlib.h>
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
	coll_cmp_f key_cmp;
};

static int item_cmp(void* litem, void* ritem) {
	hashmap_item_t* lHashMapItem = (hashmap_item_t*)litem;
	hashmap_item_t* rHashMapItem = (hashmap_item_t*)ritem;

	return lHashMapItem->owner->key_cmp(lHashMapItem->key, rHashMapItem->key);
}

static hashmap_item_t* item_create(hashmap_t* hashmap, void* key, void* item) {
	hashmap_item_t* hashMapItem = malloc(sizeof(hashmap_item_t));
	hashMapItem->key = key;
	hashMapItem->item = item;
	hashMapItem->owner = hashmap;
}

static void* item_clone(void *item) {

	return item;
}

static void item_free(void *item) {

	hashmap_item_t* hashmap_item = malloc(sizeof(hashmap_item_t));
	free(hashmap_item);
}

static unsigned long item_hash(void *item) {
	hashmap_item_t* hashMapItem = (hashmap_item_t*)item;
	hashmap_t* map = hashMapItem->owner;

	return map->key_hash(hashMapItem->key);
}

static unsigned long item_rehash(void *item) {

	unsigned long hash = item_hash(item);
	return hash * 37;
}

hashmap_t* hashmap_create(coll_hash_f key_hash,coll_cmp_f key_cmp) {

	hashmap_t* map = malloc(sizeof(hashmap_t));
	map->hashtable = hshinit(&item_hash, &item_rehash, &item_cmp, &item_clone, &item_free, 0);

	return map;
}


void* hashmap_put(hashmap_t* hashmap, void* key, void* item) {

	// item can be NULL
	assert(hashmap!=NULL);
	assert(key!=NULL);

	hashmap_item_t* hashMapItem = item_create(hashmap, key, item);
	hshinsert(hashmap->hashtable, hashMapItem);

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



