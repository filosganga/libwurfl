/*
 * cqueue.c
 *
 *  Created on: Jan 25, 2010
 *      Author: filosganga
 */

#include "cqueue.h"

#include <stdio.h>
#include <memory.h>
#include <math.h>

struct _cqueue_t {
	char* data;
	size_t size;
	size_t reads;
	size_t writes;
};

size_t min_size(size_t left, size_t right) {
	return left<right?left:right;
}

size_t get_tail(cqueue_t* cqueue) {
	size_t tail = cqueue->writes%cqueue->size;

	return tail;
}

size_t get_end(cqueue_t* cqueue) {
	size_t end = cqueue->size - 1;

	return end;
}

size_t get_head(cqueue_t* cqueue) {

	size_t head = cqueue->reads%cqueue->size;

	return head;
}


size_t get_available_data(cqueue_t* cqueue) {

	return cqueue->writes - cqueue->reads;
}

size_t get_available_space(cqueue_t* cqueue) {

	return cqueue->size - get_available_data(cqueue);
}

bool is_empty(cqueue_t* cqueue) {

	return get_available_data(cqueue) == 0;
}

cqueue_t* cqueue_create(size_t size) {

	cqueue_t* cqueue = malloc(sizeof(cqueue_t));

	cqueue->data = malloc(size);

	cqueue->size = size;
	cqueue->reads=0;
	cqueue->writes=0;

	return cqueue;
}

void cqueue_destroy(cqueue_t* cqueue) {

	free(cqueue->data);
	free(cqueue);
}

void cqueue_write(cqueue_t* cqueue, char* buffer, size_t size) {

	if(get_available_space(cqueue)<size) {
		// TODO: Error
	}

	char* bytes = (char*)buffer;
	size_t tail = get_tail(cqueue);

	size_t to_write = min_size(tail + size, cqueue->size - tail);
	memcpy((cqueue->data + tail), bytes, to_write);

	fprintf(stderr, "size: %d, tail: %d, cqueue->size: %d, to_write: %d\n", size, tail, cqueue->size, to_write);

	if(to_write < size) {
		bytes = bytes + to_write;
		to_write = size - to_write;
		memcpy(cqueue->data, bytes, to_write);
	}

	cqueue->writes+=size;
}

void cqueue_read(cqueue_t* cqueue, char* buffer, size_t size) {

	if(get_available_data(cqueue)<size) {
		// TODO: Error
	}

	char* bytes = (char*)buffer;
	size_t head = get_head(cqueue);

	size_t to_read = min_size(head + size, cqueue->size - head);
	memcpy(bytes, (cqueue->data + head), to_read);

	if(to_read < size) {
		bytes += to_read;
		to_read = size - to_read;
		memcpy(bytes, cqueue->data, to_read);
	}

	cqueue->reads+=size;
}

char cqueue_getc(cqueue_t* cqueue){

	char buffer[1];

	cqueue_read(cqueue, buffer, 1);

	return buffer[0];
}

void cqueue_putback(cqueue_t* cqueue, char* buffer, size_t size) {

	if(get_available_space(cqueue)<size) {
		// TODO: Error
	}

	char* bytes = (char*)buffer;
	size_t head = get_head(cqueue);

	size_t start = head + (cqueue->size - size);
	size_t to_putback = min_size(size, cqueue->size - start);

	memcpy((cqueue->data + start), bytes, to_putback);

	if(to_putback<size) {
		bytes += to_putback;
		to_putback = size - to_putback;
		memcpy(cqueue->data, bytes, to_putback);
	}

	cqueue->reads -= size;
}

void cqueue_putbackchar(cqueue_t* cqueue, char c) {

	char buffer[1];
	buffer[0] = c;

	cqueue_putback(cqueue, buffer, 1);
}

void cqueue_status(cqueue_t* cqueue, cqueue_status_t* status) {

	status->available_data = get_available_data(cqueue);
	status->available_space = get_available_space(cqueue);
	status->empty = is_empty(cqueue);
	status->size = cqueue->size;
}

bool cqueue_empty(cqueue_t* cqueue) {

	return is_empty(cqueue);
}

size_t cqueue_size(cqueue_t* cqueue) {

	return cqueue->size;
}

size_t cqueue_available_size(cqueue_t* cqueue) {

	return get_available_space(cqueue);
}

size_t cqueue_available_data(cqueue_t* cqueue) {

	return get_available_data(cqueue);
}
