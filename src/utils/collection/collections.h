#ifndef COLLECTIONS_H_
#define COLLECTIONS_H_

#include <stdbool.h>

/* Possible error returns, powers of 2 */
enum coll_errors {COLL_OK = 0, COLL_NOMEM=2};

typedef int (*coll_functor_f)(const void* item, void* data);

typedef struct {
	coll_functor_f functor;
	void* data;
} coll_functor_t;

typedef int (*coll_predicate_f)(const void* item, void* data);

typedef struct {
	coll_predicate_f predicate;
	void* data;
} coll_predicate_t;

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
 * Check if twoo items are equals.
 *
 * @param litem Left item
 * @param ritem Right item
 *
 * @return !=0 if the items are equals, 0 otherwise
 */
typedef int (*coll_equals_f)(const void* litem, const void *ritem);

/**
 * Type of function used to calculate hashing of an item.
 */
typedef unsigned long (*coll_hash_f)(void *item);

/**
 * Calculate the hash for a string.
 *
 * @param string The string to calculate hash from.
 * @return hashing code obtained from the given string
 */
unsigned long string_hash(const void* string);

/**
 * Calculate the hash for a string (rehash).
 *
 * @param string The string to calculate hash from.
 * @return hashing code obtained from the given string
 */
unsigned long string_rehash(const void* string);

int string_cmp(const void* litem, const void *ritem);

bool string_eq(const void *litem, const void *ritem);

bool ref_eq(const void* litem, const void *ritem);

unsigned long int_hash(const void* item);

unsigned long int_rehash(const void* item);

int int_cmp(const void* litem, const void *ritem);

int int_eq(const void* litem, const void *ritem);

#endif /*COLLECTIONS_H_*/
