/*
 * cqueue.h
 *
 *  Created on: Jan 25, 2010
 *      Author: filosganga
 */

#ifndef CQUEUE_H_
#define CQUEUE_H_

#include "memory/allocator.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	bool empty;
	size_t size;
	size_t available_space;
	size_t available_data;
} cqueue_status_t;

typedef struct _cqueue_t cqueue_t;

cqueue_t* cqueue_create(allocator_t* allocator, size_t size);

void cqueue_destroy(cqueue_t* cqueue);

void cqueue_write(cqueue_t* cqueue, char* buffer, size_t size);

void cqueue_read(cqueue_t* cqueue, char* buffer, size_t size);

char cqueue_getc(cqueue_t* cqueue);

void cqueue_putback(cqueue_t* cqueue, char* buffer, size_t size);

void cqueue_putbackchar(cqueue_t* cqueue, char c);

void cqueue_status(cqueue_t* cqueue, cqueue_status_t* status);

bool cqueue_empty(cqueue_t* cqueue);

size_t cqueue_size(cqueue_t* cqueue);

size_t cqueue_available_size(cqueue_t* cqueue);

size_t cqueue_available_data(cqueue_t* cqueue);

#endif /* CQUEUE_H_ */
