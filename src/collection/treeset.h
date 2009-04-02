/*
 * treeset.h
 *
 *  Created on: 27-mar-2009
 *      Author: Filippo De Luca
 */

#ifndef TREESET_H_
#define TREESET_H_

#include <sys/types.h>

typedef struct _treeset_t treeset_t;

treeset_t* treeset_create(
		coll_cmp_f item_cmp,
		coll_clone_item item_clone, coll_free_item item_free);

void* treeset_put(treeset_t* treeset, void* item);

void* treeset_get(treeset_t* treeset, void* item);

void* treeset_remove(treeset_t* treeset, void* item);

u_int32_t treeset_size(treeset_t* treeset);

int treeset_empty(treeset_t* treeset);

int treeset_contains(treeset_t* treeset, void* item);

void treeset_clear(treeset_t* treeset);

void treeset_destroy(treeset_t* treeset);

void** treeset_toarray(treeset_t* treeset);

void* treeset_find(treeset_t* treeset, coll_predicate_t* predicate);

int treeset_foreach(treeset_t* treeset, coll_functor_t* functor);

hashset_t* treeset_select(treeset_t* treeset, coll_predicate_t* predicate);

#endif /* TREESET_H_ */
