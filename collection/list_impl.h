#ifndef LIST_IMPL_H_
#define LIST_IMPL_H_

#include "list.h"

struct _ListNode
{
	void* item;
	struct _ListNode* next;
	struct _ListNode* prev;
};

typedef struct _ListNode ListNode;

struct _List
{
	ListNode* start;
	ListNode* end;

	/* functions */
	coll_cmp cmpItem;
	coll_cloneItem cloneItem;
	coll_freeItem freeItem;

	/* state */
	int size;
	ListStats stats;
};

/* Libera la memoria precedentemente allocata tramite list_allocate().
 * pre: list!= NULL, list deve puntare ad un area allocata tramite list_allocate() */
void list_free(List* list);

/* Alloca un nodo della lista.
 * pre: list!=NULL
 * return: puntatore alla memoria allocata */
ListNode* list_allocateNode(List* list, const void* item);

/* Libera la memoria allocata precedentemente dalla funzione list_allocateNode().
 * pre: list!=NULL, node!=NULL, node deve puntare ad un area precedentemente allocata da list_allocateNode() */
void list_freeNode(List* list, ListNode* node);

/* Restituisce il nodo all'indice index.
 * pre: list!=NULL, index<list->size.
 * return: un puntatore al nodo all'indice index. */
ListNode* list_getNodeAtIndex(List* list, unsigned int index);

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
