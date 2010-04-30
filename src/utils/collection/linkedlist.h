/**
 * This is a linked list interface.
 *
 * @author Filippo De Luca
 * @version $Id$
 */
#ifndef LIST_H_
#define LIST_H_

#include "collections.h"

#include "../memory/allocator.h"

#include <stdint.h>

/* Possible error returns, powers of 2 */
enum list_err {LST_OK = 0, LST_NOMEM = 2};

/**
 * The linkedlist_t type.
 *
 * It represent a linked list.
 */
typedef struct _linkedlist_t linkedlist_t;

typedef struct _linkedliststatus_t {
   enum list_err errors;
   unsigned int version;
} linkedliststatus_t;


/**
 * Create a new linkedlist_t.
 *
 * @param allocator The allocator_t used to allocate list and nodes.
 * @param item_equals Function used to compare two items.
 *
 * @return Pointer to created linkedlist_t
 */
linkedlist_t* linkedlist_create(allocator_t* allocator, coll_equals_f item_equals);

/**
 * Call list_clear() and deallocate memory from list.
 *
 * @attention After calling this function, the given linkedlist_t points to invalid location.
 */
void linkedlist_destroy(linkedlist_t*);

/**
 * Add a item to the given linkedlist_t
 *
 * TODO: is the return ok?
 *
 * @param list The linkedlist_t to add item to.
 * @param item The item to add.
 *
 * @return: 0 if no errors, -1 otherwise
 */
int linkedlist_add(linkedlist_t* list, const void* item);

/**
 * Insert a item in specified position, shift all items

 * @param list The linkedlist_t to insert item to.
 * @param index The index to insert the item at.
 * @param item The item to insert.
 *
 * @return: 0 if no errors, -1 otherwise
 * */
int linkedlist_addat(linkedlist_t* list, uint32_t index, const void* item);

/**
 * Remove first item from the list, result equals(curr_item, item) == TRUE
 */
int linkedlist_remove(linkedlist_t* list, const void* item);

/**
 * Remove item at index position, shift other items.
 */
void* linkedlist_removeat(linkedlist_t* list, uint32_t index);

/**
 * Get item at position index.
 */
void* linkedlist_get(linkedlist_t* list, uint32_t index);

/**
 * Get first occurence of item, result equals(curr_item, item) == TRUE
 *
 * @return: index of first occurence of item, -1 if item isn't in list
 */
uint32_t linkedlist_index(linkedlist_t* list, const void* item);

/**
 * Get last occurence of item, result equals(curr_item, item) == TRUE
 *
 * @return: index of first occurence of item, -1 if item isn't in list
 */
uint32_t linkedlist_last_index(linkedlist_t* list, const void* item);

/**
 * Return 0 if List pointed by list is empty
 *
 * @return: o if the list is empty, !=0 otherwise
 */
int linkedlist_empty(linkedlist_t* list);

/**
 * Return the List's size
 *
 * @return: number of list's items
 */
uint32_t linkedlist_size(linkedlist_t* list);

/**
 * Remove all items from list, and deallocate the memory to store its.
 *
 * @param list The linkedlist_t to be cleared.
 */
void linkedlist_clear(linkedlist_t* list);

/**
 * Get list's usage statistics an status info
 */
linkedliststatus_t linkedlist_status(linkedlist_t* list);

void linkedlist_to_array(linkedlist_t* list, void** array);

void* linkedlist_find(linkedlist_t* list, coll_predicate_t* predicate);

int linkedlist_foreach(linkedlist_t* list, coll_functor_t* functor);

linkedlist_t* linkedlist_select(linkedlist_t* list, coll_predicate_t* predicate);

#endif /*LIST_H_*/
