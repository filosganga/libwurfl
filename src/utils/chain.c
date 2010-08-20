/*
 * chain.c
 *
 *  Created on: Feb 23, 2010
 *      Author: filosganga
 */

#include "chain.h"

#include "utils/memory/allocator.h"
#include "collection/linkedlist.h"

#include "assert.h"

struct _chain_t {
	linkedlist_t* handlers;
	allocator_t* allocator;
};

chain_t* chain_create(allocator_t* allocator) {

	chain_t* chain = allocator_alloc(allocator, sizeof(chain_t));
	chain->allocator = allocator;

	chain->handlers = linkedlist_create(&ref_eq);

	return chain;

}

void chain_destroy(chain_t* chain) {

	linkedlist_destroy(chain->handlers);
	allocator_free(chain->allocator, chain);
}

void chain_add_handler(chain_t* chain, handler_t* handler) {

	assert(chain!=NULL);
	assert(handler!=NULL);

	linkedlist_add(chain->handlers, handler);
}

void chain_remove_handler(chain_t* chain, handler_t* handler) {

	assert(chain!=NULL);
	assert(handler!=NULL);

	linkedlist_remove(chain->handlers, handler);
}

int chain_execute(chain_t* chain, void* context) {

	uint32_t index = 0;
	uint32_t handlers_size = linkedlist_size(chain->handlers);
	handler_result_t handler_result = CHN_CONTINUE;

	while(handler_result == CHN_CONTINUE && index<handlers_size) {
		handler_t* handler = linkedlist_get(chain->handlers, index);
		handler_result = handler->execute(context, handler->data);
	}

	if(handler_result==CHN_CONTINUE) {
		return -1;
	}

	else return 0;
}
