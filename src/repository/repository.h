/*
 * repository.h
 *
 *  Created on: 23-mar-2009
 *      Author: Filippo De Luca
 */

#ifndef REPOSITORY_H_
#define REPOSITORY_H_

#include "device.h"
#include "utils/collection/collections.h"

#include <stdint.h>

typedef struct _repository_t repository_t;

repository_t* repository_create(const char* root, const char** patches);

void repository_reload(repository_t* repository, const char* root, const char** patches);

void repository_patch(repository_t* repository, const char** patches);

void repository_destroy(repository_t* repository);

device_t* repository_get(repository_t* repository, const char* id);

uint32_t repository_size(repository_t* repository);

int repository_foreach(repository_t* repository, coll_functor_t* functor);

#endif /* REPOSITORY_H_ */
