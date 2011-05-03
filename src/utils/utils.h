/* Copyright (C) 2011 Fantayeneh Asres Gizaw, Filippo De Luca
 *  
 * This file is part of libWURFL.
 *
 * libWURFL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or 
 * any later version.
 *
 * libWURFL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */
 
/* Written by Filippo De Luca <me@filippodeluca.com>.  */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h>
#include <stdint.h>

#ifndef min
#define min(a,b) ((a)<(b))?(a):(b)
#endif

#ifndef max
#define max(a,b) ((a)>=(b))?(a):(b)
#endif

/* Possible error returns, powers of 2 */
enum coll_errors {COLL_OK = 0, COLL_NOMEM=2};

/**
 * Unduper function
 */
typedef void* (coll_duper_f)(const void* item, const void* xtra);

/**
 * Unduper function
 */
typedef void (coll_unduper_f)(void* item, const void* xtra);

/**
 * Functor function
 */
typedef bool (coll_functor_f)(const void* item, void* data);


/**
 * Predicate function
 */
typedef bool (coll_predicate_f)(const void* item, void* data);


/**
 * Compare two items.
 *
 * @param litem Left item
 * @param ritem Right item
 *
 * @return <0 if litem<ritem, 0 if the items are equals, >0 if litem>ritem
 */
typedef int (coll_cmp_f)(const void* litem, const void *ritem);

/**
 * Check if two items are equals.
 *
 * @param litem Left item
 * @param ritem Right item
 *
 * @return !=0 if the items are equals, 0 otherwise
 */
typedef bool (coll_equals_f)(const void* litem, const void *ritem);

/**
 * Type of function used to calculate hashing of an item.
 *
 * @param item Item to calculate hashing from
 *
 * @return The item hashing
 */
typedef uint32_t (coll_hash_f)(const void *item);



void* coll_nop_duper(const void* item, const void* xtra);

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


char* strrev(char* dst, const char *src);

#endif /* UTILS_H_ */
