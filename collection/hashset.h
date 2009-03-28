/*
 * hashset.h
 *
 *  Created on: 27-mar-2009
 *      Author: filosganga
 */

#ifndef HASHSET_H_
#define HASHSET_H_

#include "collections.h"
#include <sys/types.h>

typedef struct _hashset_t hashset_t;

hashset_t* hasset_create(
		coll_hash item_hash, coll_hash item_rehash,
		coll_cmp item_cmp,
		coll_clone_item item_clone, coll_free_item item_free);

void* hashset_put(hashset_t* hashset, void* item);

void* hashset_get(hashset_t* hashset, void* item);

void* hashset_remove(hashset_t* hashset, void* item);

u_int32_t hashset_size(hashset_t* hashset);

int hashset_empty(hashset_t* hashset);

int hashset_contains(hashset_t* hashset, void* item);

void hashset_clear(hashset_t* hashset);

void hashset_destroy(hashset_t* hashset);


#endif /* HASHSET_H_ */
