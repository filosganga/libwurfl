/*
 * Copyright 2011 ff-dev.org
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Written by Filippo De Luca <me@filippodeluca.com>.  */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h>
#include <stdint.h>

/* Possible error returns, powers of 2 */
enum coll_errors {COLL_OK = 0, COLL_NOMEM=2};

/**
 * Unduper function
 */
typedef void (*coll_unduper_f)(void* item, const void* xtra);

/**
 * Functor function
 */
typedef int (*coll_functor_f)(const void* item, void* data);


/**
 * Predicate function
 */
typedef bool (*coll_predicate_f)(const void* item, void* data);


/**
 * Compare two items.
 *
 * @param litem Left item
 * @param ritem Right item
 *
 * @return <0 if litem<ritem, 0 if the items are equals, >0 if litem>ritem
 */
typedef int (*coll_cmp_f)(const void* litem, const void *ritem);

/**
 * Check if two items are equals.
 *
 * @param litem Left item
 * @param ritem Right item
 *
 * @return !=0 if the items are equals, 0 otherwise
 */
typedef bool (*coll_equals_f)(const void* litem, const void *ritem);

/**
 * Type of function used to calculate hashing of an item.
 *
 * @param item Item to calculate hashing from
 *
 * @return The item hashing
 */
typedef uint32_t (*coll_hash_f)(const void *item);



void coll_nop_unduper(void* item, const void* xtra);

void coll_default_unduper(void* item, const void* xtra);

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
