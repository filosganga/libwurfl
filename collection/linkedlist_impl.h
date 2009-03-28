#ifndef LIST_IMPL_H_
#define LIST_IMPL_H_

#include "linkedlist.h"

struct _linkedlist_node_t
{
	void* item;
	struct _linkedlist_node_t* next;
	struct _linkedlist_node_t* prev;
};

typedef struct _linkedlist_node_t linkedlist_node_t;

struct _linkedlist_t
{
	linkedlist_node_t* start;
	linkedlist_node_t* end;

	/* functions */
	coll_cmp cmpItem;
	coll_clone_item cloneItem;
	coll_free_item freeItem;

	/* state */
	u_int32_t size;
	linkedliststatus_t stats;
};

/* Libera la memoria precedentemente allocata tramite list_allocate().
 * pre: list!= NULL, list deve puntare ad un area allocata tramite list_allocate() */
void list_free(linkedlist_t* list);

/* Alloca un nodo della lista.
 * pre: list!=NULL
 * return: puntatore alla memoria allocata */
linkedlist_node_t* linkedlist_allocate_node(linkedlist_t* list, const void* item);

/* Libera la memoria allocata precedentemente dalla funzione list_allocateNode().
 * pre: list!=NULL, node!=NULL, node deve puntare ad un area precedentemente allocata da list_allocateNode() */
void linkedlist_free_node(linkedlist_t* list, linkedlist_node_t* node);

/* Restituisce il nodo all'indice index.
 * pre: list!=NULL, index<list->size.
 * return: un puntatore al nodo all'indice index. */
linkedlist_node_t* linkedlist_get_nodeat(linkedlist_t* list, u_int32_t index);

///* Aggiunge un nodo in cima alla lista
// * pre: list != NULL */
//int list_addNodeToHead(List* list, const void* item);
//
///* Aggiunge un nodo in coda alla lista
// * pre: list != NULL */
//int list_addNodeToTail(List* list, const void* item);
//
///* Aggiunge un nodo all'indice puntato da index, facendo scorrere i seguenti nodi.
// * se index=0, si comporta come list_addNodeToHead(), se index = list_getSize(list) si comporta come list_addNodeToTail().
// * pre: list!=NULL, index<=list_size(list) */
//int list_addNodeAtIndex(List* list, const void* item, unsigned int index);
//
///* Rimuove il primo nodo, facendo scorrere in avanti i seguenti
// * pre: list!=NULL */
//void* list_removeNodeFromHead(List* list);
//
///* Rimuove l'ultimo nodo.
// * pre list != NULL. */
//void* list_removeNodeFromTail(List* list);
//
///* Rimuove il nodo all'indice puntato da index.
// * pre: list!=NULL, index < list->size. */
//void* list_removeNodeAtIndex(List* list, unsigned int index);

#endif /*LIST_IMPL_H_*/
