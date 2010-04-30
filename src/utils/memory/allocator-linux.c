/*
 * allocator.c
 *
 *  Created on: Jan 20, 2010
 *      Author: filosganga
 */
#ifdef __linux__
#include "allocator.h"

#include "gnulib/error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <search.h>
#include <memory.h>
#include <errno.h>

typedef struct _allocator_node_t allocator_node_t;

typedef struct _allocator_child_t allocator_child_t;

struct _allocator_t {
	allocator_t* parent;
	allocator_node_t* node;
	allocator_child_t* first_child;
};

struct _allocator_node_t {
	allocator_node_t* next;
	void* data;
};

struct _allocator_child_t {
	allocator_child_t* next;
	allocator_t* allocator;
};

static int compare(const void* left, const void* right) {
	return left==right;
}

enum allocator_error {ALLOC_OK=0, ALLOC_NOMEM=2};


void allocator_remove_from_parent(allocator_t* allocator) {

	if(allocator->parent!=NULL) {
		// TODO remove from parent
	}
}

void allocator_destroy_children(allocator_t* allocator) {

	allocator_child_t* child = allocator->first_child;
	while(child!=NULL) {
		allocator_destroy(child);
	}
}

allocator_node_t* allocator_node_create(size_t size) {

	allocator_node_t* node;

	if(!(node=malloc(sizeof(allocator_node_t)))){
		error(ALLOC_NOMEM, errno, "Error allocating node");
	}

	if(!(node->data=malloc(size))){
		error(ALLOC_NOMEM, errno, "Error allocating node data");
	}

	node->next=NULL;

	return node;
}

void allocator_node_destroy(allocator_node_t* node) {

	free(node->data);
	free(node);
}

void allocator_node_destroyall(allocator_node_t* node) {
	if(node->next) {
		allocator_node_destroyall(node->next);
	}

	allocator_node_destroy(node);
}

allocator_child_t* allocator_child_create() {

	allocator_child_t* child;

	if(!(child = malloc(sizeof(allocator_child_t)))) {
		error(ALLOC_NOMEM, errno, "Error allocating child");
	}

	child->next = NULL;

	return child;
}

void allocator_addchild(allocator_t* parent, allocator_t* child) {


	allocator_child_t* child_node = allocator_child_create();

	child->parent = parent;

	if(parent->first_child==NULL){
		parent->first_child = child_node;
	}
	else {
		allocator_child_t* curr_child_node = parent->first_child;
		while(curr_child_node->next != NULL) {
			curr_child_node = curr_child_node->next;
		}

		curr_child_node->next = child_node;
	}
}

void allocator_remchild(allocator_t* parent, allocator_t* child) {

	if(parent->first_child==NULL) {
		error(0,0, "The allocator has no children");
	}

	if(parent->first_child == child) {

		parent->first_child = parent->first_child->next;
	}
	else {
		allocator_child_t* curr_child_node = parent->first_child;
		while(curr_child_node->next->allocator != child) {
			curr_child_node = curr_child_node->next;
		}

		// TODO ...
	}
}

allocator_t* allocator_create() {

	allocator_t* allocator;

	if(!(allocator=malloc(sizeof(allocator_t)))) {
		error(ALLOC_NOMEM, errno, "Error allocating allocator");
	}

	allocator->node = NULL;

	return allocator;
}

allocator_t* allocator_screate(allocator_t* parent) {

	allocator_t* allocator;

	if(!(allocator=malloc(sizeof(allocator_t)))) {
		error(ALLOC_NOMEM, errno, "Error allocating allocator");
	}

	if(parent!=NULL) {
		allocator_addchild(parent, allocator);
	}

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
	free(allocator);
}

#endif
