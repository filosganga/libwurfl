#include "hashmap.h"
#include "hashlib.h"

#include <stdlib.h>
#include <string.h>

typedef struct _hashmap_item_t {
	void *key;
	void* item;
	HashMap* owner;
} hashmap_item_t;

struct _HashMap {
	hshtbl* hashtable;
	coll_hash key_hash;
	coll_hash key_rehash;
	coll_cloneItem key_clone;
	coll_freeItem key_free;
	coll_cmp key_cmp;
	coll_cloneItem item_clone;
	coll_freeItem item_free;
};

static int hashtable_cmp(void* litem, void* ritem) {
	hashmap_item_t* lHashMapItem = (hashmap_item_t*)litem;
	hashmap_item_t* rHashMapItem = (hashmap_item_t*)ritem;

	return lHashMapItem->owner->key_cmp(lHashMapItem->key, rHashMapItem->key);
}

static void* hashtable_create(HashMap* hashmap, void* key, void* item) {
	hashmap_item_t* hashMapItem = malloc(sizeof(hashmap_item_t));
	hashMapItem->key = hashmap->key_clone(key);
	hashMapItem->item = hashmap->item_clone(item);
	hashMapItem->owner = hashmap;
}

static void *hashtable_clone(void *item) {

	hashmap_item_t* toClone = (hashmap_item_t*)item;
	HashMap* map = toClone->owner;

	hashmap_item_t* hashMapItem = malloc(sizeof(hashmap_item_t));
	hashMapItem->key = map->key_clone(toClone->key);
	hashMapItem->item = map->item_clone(toClone->item);
	hashMapItem->owner = toClone->owner;

	return hashMapItem;
}

static void hashtable_free(void *item) {
	hashmap_item_t* hashMapItem = (hashmap_item_t*)item;
	HashMap* map = hashMapItem->owner;

	map->key_free(hashMapItem->key);
	map->item_free(hashMapItem->item);

	free(hashMapItem);
}

static unsigned long hashtable_hash(void *item) {
	hashmap_item_t* hashMapItem = (hashmap_item_t*)item;
	HashMap* map = hashMapItem->owner;

	return map->key_hash(hashMapItem->key);
}

static unsigned long hashtable_rehash(void *item) {
	hashmap_item_t* hashMapItem = (hashmap_item_t*)item;
	HashMap* map = hashMapItem->owner;

	return map->key_rehash(hashMapItem->key);
}

HashMap* hashmap_create(
		coll_hash key_hash, coll_hash key_rehash,
		coll_cmp key_cmp,
		coll_cloneItem key_clone, coll_freeItem key_free,
		coll_cloneItem item_clone, coll_freeItem item_free) {

	HashMap* map = malloc(sizeof(HashMap));
	map->hashtable = hshinit(&hashtable_hash, &hashtable_rehash, &hashtable_cmp, &hashtable_clone, &hashtable_free, 0);

	return map;
}


void* hashmap_put(HashMap* hashmap, void* key, void* item) {

	hashmap_item_t* hashMapItem = malloc(sizeof(hashmap_item_t));
	hashMapItem->item = item;
	hashMapItem->key = key;
	hashMapItem->owner = hashmap;

	hshinsert(hashmap->hashtable, hashMapItem);

	hashtable_free(hashMapItem);

	return item;
}

void* hashmap_get(HashMap* hashmap, void* key) {

	void* item = NULL;

	hashmap_item_t hashMapItem;
	hashMapItem.key = key;
	hashMapItem.owner = hashmap;

	hashmap_item_t* foundItem = hshfind(hashmap->hashtable, &hashMapItem);

	if(foundItem){
		item = foundItem->item;
	}

	return item;
}

void* hashmap_remove(HashMap* hashmap, void* key) {

	void* item = NULL;

	hashmap_item_t hashMapItem;
	hashMapItem.key = key;
	hashMapItem.owner = hashmap;

	hashmap_item_t* removed = hshdelete(hashmap->hashtable, &hashMapItem);

	if(removed){
		item = hashmap->item_clone(removed->item);
		hashtable_free(removed);
	}

	return item;
}

unsigned long hashmap_size(HashMap* hashmap){

	return hshstatus(hashmap->hashtable).hentries;
}

int hashmap_empty(HashMap* hashmap) {

	return hashmap_size(hashmap)>0;
}

int hashmap_contain_key(HashMap* hashmap, void* key) {
	return hashmap_get(hashmap, key) != NULL;
}

void hashmap_clear(HashMap* hashmap) {
	hshkill(hashmap->hashtable);
	hashmap->hashtable = hshinit(&hashtable_hash, &hashtable_rehash, &hashtable_cmp, &hashtable_clone, &hashtable_free, 0);
}

void hashmap_destroy(HashMap* hashmap) {

	hshkill(hashmap->hashtable);
	free(hashmap);
}



