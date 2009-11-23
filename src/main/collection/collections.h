#ifndef COLLECTIONS_H_
#define COLLECTIONS_H_

#include <sys/types.h>

/* Possible error returns, powers of 2 */
enum coll_errors {COLL_OK = 0, COLL_NOMEM};

typedef int (*coll_functor_f)(const void* item, void* data);

typedef struct {
	coll_functor_f functor;
	void* data;
} coll_functor_t;

typedef int (*coll_predicate_f)(const void*, void* data);

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
 * @return -1 if litem<ritem, 0 if the items are equals, 1 if litem>ritem
 */
typedef int (*coll_cmp_f)(const void* litem, const void *ritem);

typedef int (*coll_equals_f)(const void* litem, const void *ritem);

typedef unsigned long (*coll_hash_f)(void *item);

unsigned long string_hash(const void* string);

unsigned long string_rehash(const void* string);

int string_cmp(const void* litem, const void *ritem);

int string_equals(const void* litem, const void *ritem);


unsigned long hash_int(const void* item);

unsigned long rehash_int(const void* item);

int int_cmp(const void* litem, const void *ritem);



#endif /*COLLECTIONS_H_*/
