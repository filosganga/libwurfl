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

#ifndef LIST_H_
#define LIST_H_

#include "utils.h"

#include <stdint.h>

/* Possible error returns, powers of 2 */
enum list_err {LST_OK = 0, LST_NOMEM = 2};

/**
 * The linkedlist_t type.
 *
 * It represent a linked list.
 */
typedef struct _linkedlist_t linkedlist_t;

typedef struct {
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
linkedlist_t* linkedlist_create(coll_equals_f* item_equals);

/**
 * Call list_clear() and deallocate memory from list.
 *
 * @attention After calling this function, the given linkedlist_t points to invalid location.
 */
void linkedlist_destroy(linkedlist_t*, coll_unduper_f* unduper, void* unduper_data);

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
void* linkedlist_remove(linkedlist_t* list, const void* item);

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
void linkedlist_clear(linkedlist_t* list, coll_unduper_f* unduper, void* unduper_data);

/**
 * Get list's usage statistics an status info
 */
linkedliststatus_t linkedlist_status(linkedlist_t* list);

//void* linkedlist_find(linkedlist_t* list, coll_predicate_f* predicate, void* predicate_data);

int linkedlist_foreach(linkedlist_t* list, coll_functor_f* functor, void* functor_data);


void linkedlist_toarray(linkedlist_t* list, void** array);

#endif /*LIST_H_*/
