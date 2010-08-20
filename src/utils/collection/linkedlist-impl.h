#ifndef LIST_IMPL_H_
#define LIST_IMPL_H_

#include "linkedlist.h"

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
	coll_equals_f item_equals;

	/* state */
	uint32_t size;
	linkedliststatus_t stats;
};

/* Libera la memoria precedentemente allocata tramite list_allocate().
 * pre: list!= NULL, list deve puntare ad un area allocata tramite list_allocate() */
void linkedlist_free(linkedlist_t* list);

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
