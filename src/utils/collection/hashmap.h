/*
 * hashmap.h
 *
 *  Created on: 27-mar-2009
 *      Author: filosganga
 */

#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "collections.h"
#include <stdlib.h>
#include <stdint.h>

typedef struct _hashmap_t hashmap_t;

typedef struct {
	uint32_t initial_capacity;
	float load_factor;
} hashmap_options_t;

hashmap_t* hashmap_create(coll_equals_f key_equals, coll_hash_f key_hash, hashmap_options_t* options);

void hashmap_destroy(hashmap_t* hashmap);


void* hashmap_get(hashmap_t* hashmap, const void* key);

void* hashmap_put(hashmap_t* hashmap, const void* key, const void* item);

void* hashmap_remove(hashmap_t* hashmap, const void* key);

void hashmap_clear(hashmap_t* hashmap);


uint32_t hashmap_size(hashmap_t* hashmap);

int hashmap_empty(hashmap_t* hashmap);

int hashmap_contains_item(hashmap_t* hashmap, const void* item);


int hashmap_foreach(hashmap_t* hashmap, coll_functor_t* functor);

void* hashmap_find(hashmap_t* map, coll_predicate_t* predicate, uint32_t nth);

#endif /*HASHMAP_H_*/
