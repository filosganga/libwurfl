/*
 * collections-impl.h
 *
 *  Created on: May 18, 2010
 *      Author: filosganga
 */

#ifndef COLLECTIONSIMPL_H_
#define COLLECTIONSIMPL_H_

#include "collections.h"

typedef struct {
	uint32_t nth;
	coll_predicate_t* predicate;
	void* found;
} coll_finder_data_t;

int coll_finder(const void* item, void* xtra);


#endif /* COLLECTIONSIMPL_H_ */
