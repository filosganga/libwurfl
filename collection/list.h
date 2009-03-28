#ifndef LIST_H_
#define LIST_H_

#include "collections.h"

/* Possible error returns, powers of 2 */
enum list_err {LST_OK = 0, LST_NOMEM = 2};

/* Opaque object */
typedef struct _List List;

/* NOTE: probes and misses aids evaluating hash functions       */
typedef struct _ListStats {
   enum list_err   errors;          /* error status */
   unsigned int  version;         /* hashlib version */
} ListStats;


/* Add a item to the list
 * pre: list != NULL, (item != NULL)???
 * return: 0 if no errors, -1 otherwise
 */
int list_add(List* list, const void* item);


/* Insert a item in specified position, shift all items
 * pre: list!=NULL,index<=list_size(), (item!=NULL)
 * return: 0 if no errors, -1 otherwise
 * */
int list_addAtIndex(List* list, unsigned int index, const void* item);

/* Remove first item from the list, result equals(curr_item, item) == TRUE
 * pre: list!=NULL, (item!=NULL)
 */
int list_remove(List* list, const void* item);

/* Remove item at index position, shift other items.
 * pre: list!=NULL,index<ist_size()
 */
void* list_removeAtIndex(List* list, unsigned int index);

/* Get item at position index.
 * pre: list != NULL, index < list_size()
 */
void* list_get(List* list, unsigned int index);

/* Get first occurence of item, result equals(curr_item, item) == TRUE
 * pre: list!=NULL, (item!=NULL)
 * return: index of first occurence of item, -1 if item isn't in list
 * */
int list_indexOf(List* list, const void* item);

/* Get last occurence of item, result equals(curr_item, item) == TRUE
 * pre: list!=NULL, (item!=NULL)
 * return: index of first occurence of item, -1 if item isn't in list
 * */
int list_lastIndexOf(List* list, const void* item);

/* Return 0 if List pointed by list is empty
 * pre: list != NULL;
 * return: o if the list is empty, !=0 otherwise
 * */
int list_isEmpty(List* list);

/* Return the List's size
 * pre: list!=NULL.
 * return: number of list's items
 */
int list_getSize(List* list);

/* Instanziate a new List object
 * equals = function used to verify is two items are equals
 * dup = function used to allocate memory to store item;
 * undup = function used to deallocate memory, allocate by dup.
 * return: a pointer to created List
 */
List* list_create(coll_cmp cmpItem, coll_cloneItem cloneItem, coll_freeItem freeItem);

/* Remove all items from list, and deallocate the memory to store its.
 * pre: list!= NULL.
 */
void list_clear(List* list);

/* Call list_clear() and deallocate memory from list.
 * post: list point a invalid location
 */
void list_destroy(List*);

/* Get list's usage statistics an status info
 * pre: list != NULL
 */
ListStats list_getStatus(List* list);


/* TODO */
void** list_toArray(List* list, void* array[]);

#endif /*LIST_H_*/
