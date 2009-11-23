#ifndef LIST_H_
#define LIST_H_

#include "collections.h"

/* Possible error returns, powers of 2 */
enum list_err {LST_OK = 0, LST_NOMEM = 2};

/* Opaque object */
typedef struct _linkedlist_t linkedlist_t;

typedef struct _linkedliststatus_t {
   enum list_err errors;
   unsigned int version;
} linkedliststatus_t;


/* Add a item to the list
 * pre: list != NULL, (item != NULL)???
 * return: 0 if no errors, -1 otherwise
 */
int linkedlist_add(linkedlist_t* list, const void* item);

/* Insert a item in specified position, shift all items
 * pre: list!=NULL,index<=list_size(), (item!=NULL)
 * return: 0 if no errors, -1 otherwise
 * */
int linkedlist_addat(linkedlist_t* list, u_int32_t index, const void* item);

/* Remove first item from the list, result equals(curr_item, item) == TRUE
 * pre: list!=NULL, (item!=NULL)
 */
int linkedlist_remove(linkedlist_t* list, const void* item);

/* Remove item at index position, shift other items.
 * pre: list!=NULL,index<ist_size()
 */
void* linkedlist_removeat(linkedlist_t* list, u_int32_t index);

/* Get item at position index.
 * pre: list != NULL, index < list_size()
 */
void* linkedlist_get(linkedlist_t* list, u_int32_t index);

/* Get first occurence of item, result equals(curr_item, item) == TRUE
 * pre: list!=NULL, (item!=NULL)
 * return: index of first occurence of item, -1 if item isn't in list
 * */
u_int32_t linkedlist_index(linkedlist_t* list, const void* item);

/* Get last occurence of item, result equals(curr_item, item) == TRUE
 * pre: list!=NULL, (item!=NULL)
 * return: index of first occurence of item, -1 if item isn't in list
 * */
u_int32_t linkedlist_last_index(linkedlist_t* list, const void* item);

/* Return 0 if List pointed by list is empty
 * pre: list != NULL;
 * return: o if the list is empty, !=0 otherwise
 * */
int linkedlist_empty(linkedlist_t* list);

/* Return the List's size
 * pre: list!=NULL.
 * return: number of list's items
 */
u_int32_t linkedlist_size(linkedlist_t* list);

/* Instanziate a new List object
 *
 * @param item_cmp Function used to compare two items.
 * @return Pointer to created List
 */
linkedlist_t* linkedlist_create(coll_equals_f item_equals);

/* Remove all items from list, and deallocate the memory to store its.
 * pre: list!= NULL.
 */
void linkedlist_clear(linkedlist_t* list);

/* Call list_clear() and deallocate memory from list.
 * post: list point a invalid location
 */
void linkedlist_destroy(linkedlist_t*);

/* Get list's usage statistics an status info
 * pre: list != NULL
 */
linkedliststatus_t linkedlist_status(linkedlist_t* list);

void** linkedlist_to_array(linkedlist_t* list);

void* linkedlist_find(linkedlist_t* list, coll_predicate_t* predicate);

int linkedlist_foreach(linkedlist_t* list, coll_functor_t* functor);

linkedlist_t* linkedlist_select(linkedlist_t* list, coll_predicate_t* predicate);

#endif /*LIST_H_*/
