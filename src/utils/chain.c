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

#include "chain.h"

#include "linkedlist.h"
#include "utils.h"

#include <assert.h>

struct _chain_t {
	linkedlist_t* handlers;
};

chain_t* chain_create() {

	chain_t* chain = malloc(sizeof(chain_t));
	chain->handlers = linkedlist_create(&ref_eq);

	return chain;
}

void chain_destroy(chain_t* chain) {

	linkedlist_destroy(chain->handlers, NULL, NULL);
	free(chain);
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
