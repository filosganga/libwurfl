#ifndef COLLECTIONS_H_
#define COLLECTIONS_H_

#include <sys/types.h>

/* Possible error returns, powers of 2 */
enum coll_errors {COLL_OK = 0, COLL_NOMEM};

/**
 * Compare two items.
 *
 * @param litem Left item
 * @param ritem Right item
 *
 * @return -1 if litem<ritem, 0 if the items are equals, 1 if litem>ritem
 */
typedef int (*coll_cmp)(const void* litem, const void *ritem);

typedef void *(*coll_clone_item)(const void *item);

typedef void (*coll_free_item)(void *item);

typedef unsigned long (*coll_hash)(void *item);


void* clone_item_nop(const void *item);

void free_item_nop(void *item);


u_int32_t string_hash(const void* string);

u_int32_t string_rehash(const void* string);

int string_cmp(const void* litem, const void *ritem);


u_int32_t hash_int(const void* item);

u_int32_t rehash_int(const void* item);

int int_cmp(const void* litem, const void *ritem);



#endif /*COLLECTIONS_H_*/
