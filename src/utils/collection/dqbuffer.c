/*
 * dqbuffer.c
 *
 *  Created on: Jan 23, 2010
 *      Author: filosganga
 */

#include "dqbuffer.h"

typedef struct _segment_t segment_t;

struct _segment_t {
	size_t data_size;
	void* data;
	segment_t* next;
	segment_t* prev;
};

struct _dqbuffer_t {


	allocator_t* allocator;

};

dqbuffer_t* dqbuffer_create(allocator_t* allocator) {

	dqbuffer_t* dqbuffer;

	dqbuffer = allocator_alloc(allocator, sizeof(dqbuffer_t));
	dqbuffer->allocator = allocator;

	// TODO

	return dqbuffer;
}

void dqbuffer_destroy(dqbuffer_t* buffer) {

	// TODO free segment.
	allocator_free(buffer->allocator, buffer);
}

void dqbuffer_put_head(dqbuffer_t* buffer, void* data, size_t data_size) {



}

void dqbuffer_put_tail(dqbuffer_t* buffer, void* data, size_t data_size) {

	segment_t* segment;

}

void dqbuffer_remove_head(dqbuffer_t* buffer, void* data, size_t data_size);

void dqbuffer_remove_tail(dqbuffer_t* buffer, void* data, size_t data_size);

size_t dqbuffer_size(dqbuffer_t* buffer);

void dqbuffer_destroy(dqbuffer_t* buffer);
