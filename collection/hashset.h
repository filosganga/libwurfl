/*
 * hashset.h
 *
 *  Created on: 27-mar-2009
 *      Author: filosganga
 */

#ifndef HASHSET_H_
#define HASHSET_H_

#include <sys/types.h>

typedef struct _HashSet HashSet;

HashSet* hasset_create(
		coll_hash item_hash, coll_hash item_rehash,
		coll_cmp item_cmp,
		coll_cloneItem item_clone, coll_freeItem item_free);

void* hashset_put(HashSet* hashset, void* item);

void* hashset_get(HashSet* hashset, void* item);

void* hashset_remove(HashSet* hashset, void* item);

u_int32_t hashset_size(HashSet* hashset);

int hashset_empty(HashSet* hashset);

int hashset_contains(HashSet* hashset, void* item);

void hashset_clear(HashSet* hashset);

void hashset_destroy(HashSet* hashset);


#endif /* HASHSET_H_ */
