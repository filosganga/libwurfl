#ifndef COLLECTIONS_H_
#define COLLECTIONS_H_

#include <stdbool.h>
#include <stdint.h>


/* Possible error returns, powers of 2 */
enum coll_errors {COLL_OK = 0, COLL_NOMEM=2};

/**
 * Unduper function
 */
typedef void (*coll_unduper_f)(void* item, const void* xtra);

/**
 * Unduper type
 */
typedef struct {
	coll_unduper_f undupe;
	const void* xtra;
} coll_unduper_t;

/**
 * Functor function
 */
typedef int (*coll_functor_f)(const void* item, void* data);

/**
 * Functor type
 */
typedef struct {
	coll_functor_f functor;
	void* data;
} coll_functor_t;


/**
 * Predicate function
 */
typedef bool (*coll_predicate_f)(const void* item, void* data);

/**
 * Predicate type
 */
typedef struct {
	coll_predicate_f evaluate;
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


#endif /*COLLECTIONS_H_*/
