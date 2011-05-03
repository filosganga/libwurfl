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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */
 
/* Written by Filippo De Luca <me@filippodeluca.com>.  */

#ifndef LIST_IMPL_H_
#define LIST_IMPL_H_

#include "linkedlist.h"

#include <stdlib.h>
#include <stdint.h>

struct _linkedlist_node_t
{
	void* item;
	struct _linkedlist_node_t* next;
	struct _linkedlist_node_t* prev;
};

typedef struct _linkedlist_node_t linkedlist_node_t;

struct _linkedlist_t {
	linkedlist_node_t* start;
	linkedlist_node_t* end;

	/* functions */
	coll_equals_f* item_equals;

	/* state */
	size_t size;
};

/* Alloca un nodo della lista.
 * pre: list!=NULL
 * return: puntatore alla memoria allocata */
linkedlist_node_t* linkedlist_node_create(linkedlist_t* list, const void* item);

/* Libera la memoria allocata precedentemente dalla funzione list_allocateNode().
 * pre: list!=NULL, node!=NULL, node deve puntare ad un area precedentemente allocata da list_allocateNode() */
void linkedlist_node_destroy(linkedlist_t* list, linkedlist_node_t* node);

/* Restituisce il nodo all'indice index.
 * pre: list!=NULL, index<list->size.
 * return: un puntatore al nodo all'indice index. */
linkedlist_node_t* linkedlist_get_nodeat(linkedlist_t* list, uint32_t index);


#endif /*LIST_IMPL_H_*/
