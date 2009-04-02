/*
 * hashmap.h
 *
 *  Created on: 27-mar-2009
 *      Author: filosganga
 */

#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "collections.h"
#include <sys/types.h>

typedef struct _hashmap_t hashmap_t;

hashmap_t* hashmap_create(coll_hash_f key_hash, coll_cmp_f key_cmp);

void* hashmap_put(hashmap_t* hashmap, void* key, void* item);

void* hashmap_get(hashmap_t* hashmap, void* key);

void* hashmap_remove(hashmap_t* hashmap, void* key);

u_int32_t hashmap_size(hashmap_t* hashmap);

int hashmap_empty(hashmap_t* hashmap);

int hashmap_contain_item(hashmap_t* hashmap, void* item);

int hashmap_contain_key(hashmap_t* hashmap, void* key);

void hashmap_clear(hashmap_t* hashmap);

void hashmap_destroy(hashmap_t* hashmap);

void** hashmap_to_array(hashmap_t* hashmap);

void* hashmap_find(hashmap_t* hashmap, coll_predicate_t* predicate);

int hashmap_foreach(hashmap_t* hashmap, coll_functor_t* functor);

hashset_t* hashmap_select(hashmap_t* hashmap, coll_predicate_t* predicate);


#endif /*HASHMAP_H_*/
