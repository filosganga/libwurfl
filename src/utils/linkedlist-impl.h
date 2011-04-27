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
