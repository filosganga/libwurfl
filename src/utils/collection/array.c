/*
 * array.c
 *
 *  Created on: Jan 20, 2010
 *      Author: filosganga
 *
 *  TODO: Synchronize it
 *  TODO: Use allocator
 */
#include "array.h"
#include "gnulib/error.h"

#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <memory.h>

struct _array_t {
	uint32_t size;
	array_status_t status;
	void** data;
};

array_t* array_create(uint32_t size) {

	array_t* array;

	assert(size>=0);

	if(!(array = malloc(sizeof(array_t)))) {
		error(COLL_NOMEM, errno, "Error allocating array");
	}

	if(!(array->data = malloc(sizeof(void*) * size))) {
		error(COLL_NOMEM, errno, "Error allocating array data");
	}

	array->size = size;

	return array;
}

void array_destroy(array_t* array) {

	assert(array !=NULL);

	free(array->data);
	free(array);
}

int array_set(array_t* array, const void* item, uint32_t index) {

	assert(array !=NULL);
	assert(index>=0);
	assert(index < array->size);

	*(array->data + index) = item;
}

void* array_get(array_t *array, uint32_t index) {

	assert(array !=NULL);
	assert(index>=0);
	assert(index < array->size);

	return *(array->data + index);
}

uint32_t array_size(array_t* array) {

	assert(array !=NULL);

	return array->size;
}

void array_clear(array_t* array) {

	int index;

	assert(array !=NULL);

	for(index=0; index<array->size; index++) {
		array->data[index]=NULL;
	}
}

array_status_t array_status(array_t* array) {

	assert(array !=NULL);

	return array->status;
}

void** array_to_array(array_t* array) {

	void** target;

	assert(array !=NULL);

	if(!(target = malloc(sizeof(void*) * array->size))) {
		error(COLL_NOMEM, errno, "Error allocating target array");
	}

	memcpy(target, array->data,array->size);

	return target;
}

void array_resize(array_t* array, uint32_t size) {

	if(!(array->data = realloc(array->data, size))){
		error(COLL_NOMEM, errno, "Error reallocating array data");
	}

	array->size = size;
}
