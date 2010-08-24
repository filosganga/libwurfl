/*
 * chain.h
 *
 * This component represents a chain of responsibility pattern.
 *
 *  Created on: Feb 23, 2010
 *      Author: Filippo De Luca
 */
#ifndef CHAIN_H_
#define CHAIN_H_

#include <stdlib.h>
#include <stdint.h>

typedef enum {
	CHN_FINISHED=1,
	CHN_CONTINUE=0
} handler_result_t;

typedef handler_result_t (*handler_execute_f)(void* context, void* data);

typedef struct {
	handler_execute_f execute;
	void* data;
} handler_t;

/** The chain_t type */
typedef struct _chain_t chain_t;

/**
 * This function create a chain using given allocator
 *
 * @param allocator allocator_t used to allocate chain.
 */
chain_t* chain_create();

void chain_destroy(chain_t* chain);

void chain_add_handler(chain_t* chain, handler_t* handler);

void chain_remove_handler(chain_t* chain, handler_t* handler);

int chain_execute(chain_t* chain, void* context);

#endif /* CHAIN_H_ */
