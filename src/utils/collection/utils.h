/*
 * utils.h
 *
 *  Created on: 24/ago/2010
 *      Author: filippodeluca
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "collections.h"

/**
 * Calculate the hash for a string.
 *
 * @param string The string to calculate hash from.
 * @return hashing code obtained from the given string
 */
uint32_t string_hash(const void* string);

int string_cmp(const void* litem, const void *ritem);

bool string_eq(const void *litem, const void *ritem);



uint32_t int_hash(const void* item);

int int_cmp(const void* litem, const void *ritem);

bool int_eq(const void* litem, const void *ritem);



uint32_t ref_hash(const void* item);

int ref_cmp(const void* litem, const void *ritem);

bool ref_eq(const void* litem, const void *ritem);

#endif /* UTILS_H_ */
