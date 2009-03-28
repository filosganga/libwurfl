#ifndef HASHMAP_H_
#define HASHMAP_H_



typedef struct _HashMap HashMap;

HashMap* hashmap_create(
		coll_hash key_hash, coll_hash key_rehash,
		coll_cmp key_cmp,
		coll_cloneItem key_clone, coll_freeItem key_free,
		coll_cloneItem item_clone, coll_freeItem item_free);

void* hashmap_put(HashMap* hashmap, void* key, void* item);

void* hashmap_get(HashMap* hashmap, void* key);

void* hashmap_remove(HashMap* hashmap, void* key);

unsigned long hashmap_size(HashMap* hashmap);

int hashmap_empty(HashMap* hashmap);

int hashmap_contain_item(HashMap* hashmap, void* item);

int hashmap_contain_key(HashMap* hashmap, void* key);

void hashmap_clear(HashMap* hashmap);

void hashmap_destroy(HashMap* hashmap);

#endif /*HASHMAP_H_*/
