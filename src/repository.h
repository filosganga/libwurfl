/*
 * repository.h
 *
 *  Created on: 23-mar-2009
 *      Author: Filippo De Luca
 */

#ifndef REPOSITORY_H_
#define REPOSITORY_H_

#include "devicedef.h"
#include "utils/utils.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct _repository_t repository_t;

repository_t* repository_create(const char* root, const char** patches);

void repository_reload(repository_t* repository, const char* root, const char** patches);

void repository_patch(repository_t* repository, const char** patches);

void repository_destroy(repository_t* repository);

devicedef_t* repository_get(repository_t* repository, const char* id);

size_t repository_size(repository_t* repository);

bool repository_foreach(repository_t* repository, coll_functor_f functor, void* functor_data);

#endif /* REPOSITORY_H_ */
