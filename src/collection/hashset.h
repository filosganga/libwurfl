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

hashset_t* hashset_create(coll_hash_f item_hash,coll_hash_f item_rehash, coll_equals_f item_equals);

void* hashset_add(hashset_t* hashset, void* item);

void* hashset_get(hashset_t* hashset, void* item);

void* hashset_remove(hashset_t* hashset, void* item);

u_int32_t hashset_size(hashset_t* hashset);

int hashset_empty(hashset_t* hashset);

int hashset_contains(hashset_t* hashset, void* item);

void hashset_clear(hashset_t* hashset);

void hashset_destroy(hashset_t* hashset);

void** hashset_to_array(hashset_t* hashset);

int hashset_foreach(hashset_t* hashset, coll_functor_f functor, void* context);


#endif /* HASHSET_H_ */
