/*
 * treeset.h
 *
 *  Created on: 27-mar-2009
 *      Author: Filippo De Luca
 */

#ifndef TREESET_H_
#define TREESET_H_

#include <sys/types.h>

typedef struct _TreeSet TreeSet;

TreeSet* treeset_create(
		coll_cmp item_cmp,
		coll_cloneItem item_clone, coll_freeItem item_free);

void* treeset_put(TreeSet* treeset, void* item);

void* treeset_get(TreeSet* treeset, void* item);

void* treeset_remove(TreeSet* treeset, void* item);

u_int32_t treeset_size(TreeSet* treeset);

int treeset_empty(TreeSet* treeset);

int treeset_contains(TreeSet* treeset, void* item);

void treeset_clear(TreeSet* treeset);

void treeset_destroy(TreeSet* treeset);

#endif /* TREESET_H_ */
