#include "linkedlist_impl.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifdef IS_WINDOWS
#include <windows.h>

HANDLE list_get_heap(linkedlist_t* list)
{

	/* TODO consider use of one separate heap for the List */

	HANDLE heap_handle;
	heap_handle = GetProcessHeap();
	if (heap_handle==NULL)
	{
		// errore
	}

	return heap_handle;
}

static linkedlist_t* allocateList() {
	HANDLE heap_handle = list_get_heap(NULL);
	linkedlist_t* list = HeapAlloc(heap_handle, 0, sizeof(linkedlist_t));

	return list;
}

static linkedlist_node_t* allocateListNode() {
	HANDLE heap_handle = list_get_heap(NULL);
	linkedlist_node_t* node = HeapAlloc(heap_handle, 0, sizeof(linkedlist_node_t));

	return node;
}

static void freeList(linkedlist_t* toRemove){
	HANDLE heap_handle = list_get_heap(NULL);

	if (!HeapFree(heap_handle, 0, list))
	{
		// errore
	}
}

static void freeListNode(linkedlist_node_t* toRemove){
	HANDLE heap_handle = list_get_heap(NULL);
	if (!HeapFree(heap_handle, 0, toRemove))
	{
		// errore
	}
}

#else // WIN32


#endif

static linkedlist_t* list_allocate() {

	linkedlist_t* list = NULL;

#ifdef _WIN32
	HANDLE heap_handle = list_get_heap(NULL);

	list = HeapAlloc(heap_handle, 0, sizeof(linkedlist_t));
#else
	list = malloc(sizeof(linkedlist_t));
#endif
	return list;
}

void list_free(linkedlist_t* list) {

#ifdef _WIN32
	HANDLE heap_handle = list_get_heap(NULL);

	if (!HeapFree(heap_handle, 0, list))
	{
		// errore
	}
#else
	free(list);
#endif
}

void linkedlist_free_node(linkedlist_t* list, linkedlist_node_t *toRemove) {

#ifdef _WIN32
	HANDLE heap_handle = list_get_heap(NULL);

	if (!HeapFree(heap_handle, 0, toRemove))
	{
		// errore
	}
#else
	free(toRemove);
#endif
}

linkedlist_node_t* linkedlist_allocate_node(linkedlist_t* list, const void *item) {
	void* duped_item;
	linkedlist_node_t* toAdd = NULL;

	duped_item = list->cloneItem(item);

	if (duped_item) {

#ifdef _WIN32
		HANDLE heap_handle = list_get_heap(NULL);
		toAdd = HeapAlloc(heap_handle, 0, sizeof(linkedlist_node_t));
#else
		toAdd = malloc(sizeof(linkedlist_node_t));
#endif
		if (toAdd) {
			toAdd->item = duped_item;
		} else {
			toAdd = NULL;
			list->stats.errors |= COLL_NOMEM;
		}
	}

	else {
		duped_item = NULL;
		list->stats.errors |= COLL_NOMEM;
	}

	return toAdd;
}

linkedlist_node_t* linkedlist_get_nodeat(linkedlist_t* list, u_int32_t index) {
	linkedlist_node_t* toGet = NULL;
	int i;

	assert(list!=NULL);

	if (list->size > index) {

		if (index < list->size / 2) {
			toGet = list->start;
			for (i = 0; i < index; i++)
				toGet = toGet->next;
		} else {
			toGet = list->end;
			for (i = list->size - 1; i > index; i--)
				toGet = toGet->prev;
		}

	} // list->size>index

	return toGet;

} // list_getNodeAtIndex

///* adding items */
//int list_addNodeToHead(List* list, const void* item)
//{
//	ListNode* toAdd = list_allocateNode(list, item);
//	int res_code;
//
//	if (toAdd)
//	{
//
//		toAdd->prev = NULL;
//		toAdd->next = NULL;
//
//		if (list->start!=NULL)
//		{
//			list->start->prev = toAdd;
//			toAdd->next = list->start;
//		}
//
//		list->start = toAdd;
//
//		if (list->end == NULL)
//		{
//			list->end = toAdd;
//		}
//
//		list->size++;
//
//	}
//	else
//	{
//		res_code = -1;
//	}
//	return res_code;
//}
//
//int list_addNodeToTail(List* list, const void* item)
//{
//	ListNode* toAdd = list_allocateNode(list, item);
//	int res_code;
//
//	if (toAdd)
//	{
//
//		/* list->start != NULL */
//		ListNode* current = list->end;
//
//		current->next = toAdd;
//		toAdd->next = NULL;
//		toAdd->prev = current;
//
//		list->end = toAdd;
//
//		list->size++;
//
//		res_code = 0;
//	}
//	else
//	{
//		res_code = -1;
//	}
//
//	return res_code;
//
//}
//
//int list_addNodeAtIndex(List* list, const void* item, unsigned int index)
//{
//	ListNode* prevNode;
//	ListNode* nextNode;
//	ListNode* toAdd;
//	unsigned int i;
//	int res_code;
//
//	toAdd->prev = NULL;
//	toAdd->next = NULL;
//
//	toAdd = list_allocateNode(list, item);
//	if (toAdd)
//	{
//
//		nextNode = list_getNodeAtIndex(list, index);
//		prevNode = nextNode->prev;
//
//		toAdd->prev = prevNode;
//		toAdd->next = nextNode;
//
//		if (prevNode!=NULL)
//		{
//			prevNode->next=toAdd;
//		}
//
//		if (nextNode!=NULL)
//		{
//			nextNode->prev = toAdd;
//		}
//
//		if(list->start==nextNode){
//			list->start = toAdd;
//		}
//
//		if(list->end==prevNode){
//			list->end = toAdd;
//		}
//
//		list->size++;
//
//		res_code = 0;
//	}
//	else
//	{
//		res_code = -1;
//	}
//
//	return res_code;
//}

///* removing items */
//void* list_removeNodeFromHead(List* list)
//{
//	ListNode* toRemove;
//	void *removedItem=NULL;
//
//	if (list->size>0)
//	{
//
//		toRemove = list->start;
//
//		list->start=toRemove->next;
//		if (list->start!=NULL)
//		{
//			list->start->prev = NULL;
//		}
//
//		list->size--;
//
//		removedItem = toRemove->item;
//		list_freeNode(list, toRemove);
//
//	} // list->size>0
//
//	return removedItem;
//} // list_removeNodeFromHead
//
//void* list_removeNodeFromTail(List* list)
//{
//	ListNode* toRemove;
//	void *removedItem=NULL;
//
//	if (list->size>0)
//	{
//
//		toRemove = list->end;
//
//		list->end=toRemove->prev;
//		if (list->end!=NULL)
//		{
//			list->end->next = NULL;
//		}
//
//		list->size--;
//
//		removedItem = toRemove->item;
//		list_freeNode(list, toRemove);
//
//	} // list->size>0
//
//	return removedItem;
//} // list_removeNodeFromTail()
//
//void* list_removeNodeAtIndex(List* list, unsigned int index)
//{
//	ListNode* toRemove;
//	ListNode* prevNode;
//	ListNode* nextNode;
//	void *removedItem=NULL;
//	unsigned int i;
//
//	// index must not be first or last
//	assert(index<list->size-1);
//	assert(index>0);
//
//	if (list->size>index)
//	{
//
//		// Obtain node
//		toRemove = list_getNodeAtIndex(list, index);
//
//		prevNode = toRemove->prev;
//		nextNode = toRemove->next;
//
//		// Unlink
//		prevNode->next = nextNode;
//		nextNode->prev = prevNode;
//
//		list->size--;
//
//		removedItem = toRemove->item;
//
//		// Free space
//		list_freeNode(list, toRemove);
//
//	} // list->size>index
//
//	return removedItem;
//}

// User interface *********************************************************

int linkedlist_add(linkedlist_t* list, const void* item) {
	return linkedlist_addat(list, list->size, item);
}

int linkedlist_addat(linkedlist_t* list, u_int32_t index, const void* item) {

	linkedlist_node_t* prevNode = NULL;
	linkedlist_node_t* nextNode = NULL;
	linkedlist_node_t* toAdd = NULL;
	int res_code;

	toAdd = linkedlist_allocate_node(list, item);
	if (toAdd) {
		toAdd->prev = NULL;
		toAdd->next = NULL;

		nextNode = linkedlist_get_nodeat(list, index);

		if (nextNode != NULL) {
			nextNode->prev = toAdd;
			prevNode = nextNode->prev;
		} else if (index > 0) {
			prevNode = linkedlist_get_nodeat(list, index - 1);
		}

		toAdd->prev = prevNode;
		toAdd->next = nextNode;

		if (prevNode != NULL) {
			prevNode->next = toAdd;
		}

		if (toAdd->prev == NULL) {
			list->start = toAdd;
		}

		if (toAdd->next == NULL) {
			list->end = toAdd;
		}

		list->size++;

		res_code = 0;
	} else {
		res_code = -1;
	}

	return res_code;

}

u_int32_t linkedlist_index(linkedlist_t* list, const void* item) {
	int nodeIndex = 0;
	linkedlist_node_t* currentNode;

	assert(list!=NULL);
	//	assert(item!=NULL);

	// Find item
	currentNode = list->start;
	while (currentNode != NULL && !list->cmpItem(currentNode->item, item)) {
		currentNode = currentNode->next;
		nodeIndex++;
	}

	/* we have reach list's end */
	if (currentNode == NULL) {
		nodeIndex = -1;
	}

	return nodeIndex;

}

u_int32_t linkedlist_last_index(linkedlist_t* list, const void* item) {
	int nodeIndex = 0;
	linkedlist_node_t* currentNode;

	assert(list!=NULL);
	//	assert(item!=NULL);

	nodeIndex = list->size - 1;

	// Find item reversed
	currentNode = list->end;
	while (nodeIndex >= 0 && !list->cmpItem(currentNode->item, item)) {
		assert(NULL != currentNode);

		currentNode = currentNode->prev;
		nodeIndex--;
	}

	return nodeIndex;

}

int linkedlist_remove(linkedlist_t* list, const void* item) {

	void* toRemove = linkedlist_removeat(list, linkedlist_index(list, item));

	return toRemove != NULL;
}

void* linkedlist_removeat(linkedlist_t* list, u_int32_t index) {

	linkedlist_node_t* toRemove;
	void *removedItem = NULL;

	assert(list!=NULL);
	assert(list->size>0);
	assert(index<list->size);

	toRemove = linkedlist_get_nodeat(list, index);

	assert(toRemove!=NULL);

	// Unlink
	if (toRemove->next != NULL) {
		toRemove->next->prev = toRemove->prev;
	}

	if (toRemove->prev != NULL) {
		toRemove->prev->next = toRemove->next;
	}

	// Update list status
	if (list->start == toRemove) {
		list->start = toRemove->next;
	} else if (list->end == toRemove) {
		list->end = toRemove->prev;
	}

	list->size--;

	// obtain nodeItem
	removedItem = toRemove->item;

	// free memory only for node not for item
	linkedlist_free_node(list, toRemove);

	return removedItem;

} // list_removeAtIndex

void* linkedlist_get(linkedlist_t* list, u_int32_t index) {
	linkedlist_node_t* toGet;

	assert(list!=NULL);
	assert(index<list->size);

	// Obtain node
	toGet = linkedlist_get_nodeat(list, index);

	return toGet->item;
} // list_get

// Modify list functions **************************************************

linkedlist_t* linkedlist_create(coll_cmp cmpItem, coll_clone_item cloneItem, coll_free_item freeItem) {

	assert(cmpItem != NULL);
	assert(cloneItem!=NULL);
	assert(freeItem!=NULL);

	linkedlist_t* list = list_allocate();
	if (list != NULL) {

		list->start = NULL;
		list->end = NULL;
		list->size = 0;
		list->cmpItem = cmpItem;
		list->cloneItem = cloneItem;
		list->freeItem = cloneItem;

		/* initialize statistics */
		list->stats.errors = COLL_OK;

	} else {
		list_free(list);
		list = NULL;
	}

	return list;
}

void linkedlist_destroy(linkedlist_t* list) {

	assert(list != NULL);

	linkedlist_clear(list);
	list_free(list);
}

void linkedlist_clear(linkedlist_t* list) {
	void* itemToFree;
	linkedlist_node_t* next = list->start;

	assert(list != NULL);

	while (next != NULL) {
		linkedlist_node_t* toFree = next;
		next = toFree->next;

		if (next != NULL) {
			next->prev = NULL;
		} else {
			list->end = NULL;
		}

		list->start = next;
		list->size--;

		itemToFree = toFree->item;

		linkedlist_free_node(list, toFree);
		list->freeItem(itemToFree);
	}

}

int linkedlist_empty(linkedlist_t* list) {

	assert(list!=NULL);

	return list->size == 0;
}

u_int32_t linkedlist_size(linkedlist_t* list) {
	assert(list!=NULL);

	return list->size;
}

linkedliststatus_t linkedlist_status(linkedlist_t* list) {
	assert(list!=NULL);

	return list->stats;
}

void** linkedlist_to_array(linkedlist_t* list, void* array[]) {
	//	int index = 0,i =0;
	//	void* tmpItem;
	//	ListNode* current;	/* TODO validate arguments */
	//
	//	assert(list!=NULL);
	//	assert(array!=NULL);
	//
	//	current = list->start;
	//
	//	while (current!=NULL)
	//	{
	//		/* TODO duplicate the item */
	//		tmpItem = list->dup(current->item);
	//		if(!tmpItem){
	//			list->undup(tmpItem);
	//			for(i=0;i<index;i++){
	//				list->undup(array[index]);
	//			}
	//			break;
	//		}
	//		else{
	//			array[index] = tmpItem;
	//			current = current->next;
	//			index ++;
	//		}
	//	}

	return NULL;

}

