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
 * along with libWURFL.  If not, see <http://www.gnu.org/licenses/>.
 */
 
/* Written by Filippo De Luca <me@filippodeluca.com>.  */

#ifndef LIST_H_
#define LIST_H_

#include "utils.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * The linkedlist_t type.
 *
 * It represent a linked list.
 */
typedef struct _linkedlist_t linkedlist_t;

/**
 * Create a new linkedlist_t.
 *
 * @param item_equals Function used to compare two items.
 *
 * @return Pointer to created linkedlist_t
 */
linkedlist_t* linkedlist_init(coll_equals_f* item_equals);

/**
 * Call list_clear() and deallocate memory from list.
 *
 * @attention After calling this function, the given linkedlist_t points to invalid location.
 */
void linkedlist_free(linkedlist_t*, coll_unduper_f* unduper, void* unduper_data);

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
bool linkedlist_empty(linkedlist_t* list);

/**
 * Return the List's size
 *
 * @return: number of list's items
 */
size_t linkedlist_size(linkedlist_t* list);

/**
 * Remove all items from list, and deallocate the memory to store its.
 *
 * @param list The linkedlist_t to be cleared.
 */
void linkedlist_clear(linkedlist_t* list, coll_unduper_f* unduper, void* unduper_data);

//void* linkedlist_find(linkedlist_t* list, coll_predicate_f* predicate, void* predicate_data);

bool linkedlist_foreach(linkedlist_t* list, coll_functor_f* functor, void* functor_data);

#endif /*LIST_H_*/
