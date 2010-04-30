/*
 * dqbuffer.h
 *
 *  Created on: Jan 23, 2010
 *      Author: filosganga
 */

#ifndef DQBUFFER_H_
#define DQBUFFER_H_

#include "../memory/allocator.h"

typedef struct _dqbuffer_t dqbuffer_t;

dqbuffer_t* dqbuffer_create(allocator_t* allocator);

void dqbuffer_put_head(dqbuffer_t* buffer, void* data, size_t data_size);

void dqbuffer_put_tail(dqbuffer_t* buffer, void* data, size_t data_size);

void dqbuffer_remove_head(dqbuffer_t* buffer, void* data, size_t data_size);

void dqbuffer_remove_tail(dqbuffer_t* buffer, void* data, size_t data_size);

size_t dqbuffer_size(dqbuffer_t* buffer);

void dqbuffer_destroy(dqbuffer_t* buffer);

#endif /* DQBUFFER_H_ */
