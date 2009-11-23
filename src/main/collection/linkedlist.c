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

static linkedlist_t* list_alloc() {

	linkedlist_t* list = NULL;

#ifdef _WIN32
	HANDLE heap_handle = list_get_heap(NULL);

	list = HeapAlloc(heap_handle, 0, sizeof(linkedlist_t));
#else
	list = malloc(sizeof(linkedlist_t));
#endif
	return list;
}

void linkedlist_free(linkedlist_t* list) {

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

void linkedlist_node_destroy(linkedlist_t* list, linkedlist_node_t *toRemove) {

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

linkedlist_node_t* linkedlist_node_create(linkedlist_t* list, const void *item) {

	linkedlist_node_t* toAdd = NULL;

#ifdef _WIN32
	HANDLE heap_handle = list_get_heap(NULL);
	toAdd = HeapAlloc(heap_handle, 0, sizeof(linkedlist_node_t));
#else
	toAdd = malloc(sizeof(linkedlist_node_t));
#endif
	if (toAdd) {
		toAdd->item = item;
	} else {
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

// User interface *********************************************************

int linkedlist_add(linkedlist_t* list, const void* item) {
	return linkedlist_addat(list, list->size, item);
}

int linkedlist_addat(linkedlist_t* list, u_int32_t index, const void* item) {

	linkedlist_node_t* prevNode = NULL;
	linkedlist_node_t* nextNode = NULL;
	linkedlist_node_t* toAdd = NULL;

	int res_code = 0;

	toAdd = linkedlist_node_create(list, item);
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

	assert(list!=NULL);
	assert(item!=NULL);

	// Find item
	int index = 0;
	linkedlist_node_t* current = list->start;

	while (current != NULL && !list->item_equals(current->item, item)) {
		current = current->next;
		index++;
	}

	/* we have reach list's end */
	if (current == NULL) {
		index = -1;
	}

	return index;
}

u_int32_t linkedlist_last_index(linkedlist_t* list, const void* item) {

	assert(list!=NULL);
	//	assert(item!=NULL);

	int index = list->size - 1;
	linkedlist_node_t* current = list->end;
	// Find item reversed

	while (index >= 0 && !list->item_equals(current->item, item)) {
		assert(NULL != current);

		current = current->prev;
		index--;
	}

	return index;
}

int linkedlist_remove(linkedlist_t* list, const void* item) {

	void* toRemove = linkedlist_removeat(list, linkedlist_index(list, item));

	return toRemove != NULL;
}

void* linkedlist_removeat(linkedlist_t* list, u_int32_t index) {

	void *removed_item = NULL;

	assert(list!=NULL);
	assert(list->size>0);
	assert(index<list->size);

	linkedlist_node_t* removed = linkedlist_get_nodeat(list, index);

	assert(removed!=NULL);

	// Unlink
	if (removed->next != NULL) {
		removed->next->prev = removed->prev;
	}

	if (removed->prev != NULL) {
		removed->prev->next = removed->next;
	}

	// Update list status
	if (list->start == removed) {
		list->start = removed->next;
	} else if (list->end == removed) {
		list->end = removed->prev;
	}

	list->size--;

	// obtain nodeItem
	removed_item = removed->item;

	// free memory only for node not for item
	linkedlist_node_destroy(list, removed);

	return removed_item;

} // list_removeAtIndex

void* linkedlist_get(linkedlist_t* list, u_int32_t index) {
	linkedlist_node_t* to_get;

	assert(list!=NULL);
	assert(index<list->size);

	// Obtain node
	to_get = linkedlist_get_nodeat(list, index);

	return to_get->item;
} // list_get

// Modify list functions **************************************************

linkedlist_t* linkedlist_create(coll_equals_f item_equals) {

	assert(item_equals != NULL);


	linkedlist_t* list = list_alloc();
	if (list != NULL) {

		list->start = NULL;
		list->end = NULL;
		list->size = 0;
		list->item_equals = item_equals;

		/* initialize statistics */
		list->stats.errors = COLL_OK;

	}

	return list;
}

void linkedlist_destroy(linkedlist_t* list) {

	assert(list != NULL);

	linkedlist_clear(list);
	linkedlist_free(list);
}

void linkedlist_clear(linkedlist_t* list) {


	assert(list != NULL);

	linkedlist_node_t* next = list->start;
	while (next != NULL) {
		linkedlist_node_t* to_free = next;
		next = to_free->next;

		if (next != NULL) {
			next->prev = NULL;
		} else {
			list->end = NULL;
		}

		list->start = next;
		list->size--;

		linkedlist_node_destroy(list, to_free);
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

void** linkedlist_to_array(linkedlist_t* list) {

	assert(list!=NULL);

	void** array = malloc(sizeof(void*) * list->size);
	int index = 0;

	linkedlist_node_t* current = list->start;

	while(current != NULL) {

		*(array + index) = current->item;\
		current = current->next;
	}

	return array;
}

