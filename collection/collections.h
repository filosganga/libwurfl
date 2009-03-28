#ifndef COLLECTIONS_H_
#define COLLECTIONS_H_

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

typedef void *(*coll_cloneItem)(const void *item);

typedef void (*coll_freeItem)(void *item);

typedef unsigned long (*coll_hash)(void *item);


#endif /*COLLECTIONS_H_*/
