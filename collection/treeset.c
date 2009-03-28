/*
 * treeset.c
 *
 *  Created on: 27-mar-2009
 *      Author: filosganga
 */
#include "treeset.h"
#include "pavl.h"

static void* treeset_malloc(struct libavl_allocator* allocator, size_t size) {

}

static void treeset_free(struct libavl_allocator* allocator, void *){

}

TreeSet* treeset_create(
		coll_cmp item_cmp,
		coll_cloneItem item_clone, coll_freeItem item_free) {

	pavl_create()

}

void* treeset_put(TreeSet* treeset, void* item);

void* treeset_get(TreeSet* treeset, void* item);

void* treeset_remove(TreeSet* treeset, void* item);

u_int32_t treeset_size(TreeSet* treeset);

int treeset_empty(TreeSet* treeset);

int treeset_contains(TreeSet* treeset, void* item);

void treeset_clear(TreeSet* treeset);

void treeset_destroy(TreeSet* treeset);
