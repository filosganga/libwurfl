/*
 * repository.h
 *
 *  Created on: 23-mar-2009
 *      Author: Filippo De Luca
 */

#ifndef REPOSITORY_H_
#define REPOSITORY_H_

#include "resource.h"
#include "devicedef.h"
#include "hierarchy.h"

#include "utils/collection/hashmap.h"
#include "utils/collection/linkedlist.h"

#include <stdint.h>

typedef struct _repository_t repository_t;

repository_t* repository_create(const char* root, const char** patches);

void repository_destroy(repository_t* repository);

devicedef_t* repository_get_device(repository_t* repository, const char* id);

uint32_t repository_size(repository_t* repository);

int repository_foreach(repository_t* repository, coll_functor_t* functor);

#endif /* REPOSITORY_H_ */
