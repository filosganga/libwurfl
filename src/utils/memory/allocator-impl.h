/*
 * allocator-impl.h
 *
 *  Created on: Mar 21, 2010
 *      Author: filosganga
 */

#ifndef ALLOCATORIMPL_H_
#define ALLOCATORIMPL_H_

#include "allocator.h"

void allocator_addchild(allocator_t* parent, allocator_t* child);

void allocator_remchild(allocator_t* parent, allocator_t* child);


#endif /* ALLOCATORIMPL_H_ */
