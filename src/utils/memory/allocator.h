/*
 * allocator.h
 *
 *  Created on: Jan 20, 2010
 *      Author: filosganga
 */

#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <stdlib.h>

typedef struct _allocator_t allocator_t;

allocator_t* allocator_create();

allocator_t* allocator_screate(allocator_t* parent);

void allocator_destroy(allocator_t* allocator);

void* allocator_alloc(allocator_t* allocator, size_t size);

void allocator_free(allocator_t* allocator, void* ptr);

void allocator_clean(allocator_t* allocator);

#endif /* ALLOCATOR_H_ */
