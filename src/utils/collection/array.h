/*
 * array.h
 *
 *  Created on: Jan 20, 2010
 *      Author: filosganga
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include "collections.h"

#include <stdlib.h>
#include <stdint.h>

/* Possible error returns, powers of 2 */
enum array_err {ARY_OK = 0, ARY_NOMEM = 2};

/* Opaque object */
typedef struct _array_t array_t;

typedef struct _array_status_t {
   enum array_err errors;
   unsigned int version;
} array_status_t;

array_t* array_create(uint32_t size);

void array_destroy(array_t* array);

int array_set(array_t* array, const void* item, uint32_t index);

void* array_get(array_t *array, uint32_t index);

uint32_t array_size(array_t* array);

void array_clear(array_t* array);

array_status_t array_status(array_t* array);

void** array_to_array(array_t* array);

#endif /* ARRAY_H_ */
