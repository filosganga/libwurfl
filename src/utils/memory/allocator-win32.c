/*
 * allocator-win32.c
 *
 *  Created on: Feb 23, 2010
 *      Author: filosganga
 */
#ifdef _WIN32

#include "allocator.h"

#include "../gnulib/error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <search.h>
#include <memory.h>
#include <errno.h>

typedef struct _allocator_node_t allocator_node_t;

struct _allocator_t {
	HANDLE heap_handle;
	allocator_node_t* node;
};


struct _allocator_node_t {
	allocator_node_t* next;
	void* data;
};

static int compare(const void* left, const void* right) {
	return left==right;
}

enum allocator_error {ALLOC_OK=0, ALLOC_NOMEM=2};

HANDLE create_heap()
{

	/* TODO consider use of one separate heap for each allocator */
	HANDLE heap_handle;
	heap_handle = GetProcessHeap();
	if (heap_handle==NULL) {
		// error
	}

	return heap_handle;
}

allocator_node_t* allocator_node_create(size_t size) {

	allocator_node_t* node;



	if(!(node=HeapAlloc(heap, 0, sizeof(allocator_node_t)))){
		error(ALLOC_NOMEM, errno, "Error allocating node");
	}

	if(!(node->data=malloc(size))){
		error(ALLOC_NOMEM, errno, "Error allocating node data");
	}

	node->next=NULL;

	return node;
}

// TODO use allocator
void allocator_node_destroy(allocator_node_t* node) {

	if (!HeapFree(allocator->heap, 0, node->data))
	{
		// errore
	}

	if (!HeapFree(allocator->heap, 0, node))
	{
		// errore
	}

}

void allocator_node_destroyall(allocator_node_t* node) {
	if(node->next) {
		allocator_node_destroyall(node->next);
	}

	allocator_node_destroy(node);
}

allocator_t* allocator_create() {

	allocator_t* allocator;

	HANDLE heap = create_heap();

	allocator = HeapAlloc(heap, 0, sizeof(allocator_t));
	allocator->heap = heap;

	allocator->node = NULL;

	return allocator;
}

void allocator_clean(allocator_t* allocator) {

	if(allocator->node) {
		allocator_node_destroyall(allocator->node);
	}
}

void allocator_free(allocator_t* allocator, void* ptr) {

	allocator_node_t* node = allocator->node;

	if(allocator->node == NULL) {
		error(0, 0, "The allocator is empty");
	}
	if(allocator->node->data == ptr) {
		node = allocator->node;
		allocator->node = allocator->node->next;
	}
	else {

		allocator_node_t *curr, *prev;

		prev = allocator->node;
		curr = prev->next;

		while(curr != NULL && curr->data!=ptr) {
			prev = curr;
			curr = curr->next;
		}

		if(curr!=NULL) {
			node = curr;
			prev->next = curr ->next;
		}
		else {
			error(0, 0, "The allocator does not contains the node");
		}
	}

	if(node!=NULL) {
		allocator_node_destroy(node);
	}
}

void* allocator_alloc(allocator_t* allocator, size_t size) {

	allocator_node_t* node = allocator_node_create(size);

	if(allocator->node==NULL) {
		allocator->node = node;
	}
	else {

		allocator_node_t *curr = allocator->node;
		while(curr->next != NULL) {
			curr = curr -> next;
		}

		curr->next=node;
	}

	return node->data;
}

void allocator_destroy(allocator_t* allocator) {
	allocator_clean(allocator);

	if (!HeapFree(allocator->heap, 0, allocator))
	{
		// errore
	}
}

#endif // _WIN32
