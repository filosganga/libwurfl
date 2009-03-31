/*
 * model.h
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */

#ifndef MODEL_H_
#define MODEL_H_

#include "resource.h"
#include "devicedef.h"
#include "hierarchy.h"

#include "../collection/hashmap.h"
#include "../collection/linkedlist.h"

typedef struct _repository_t repository_t;

repository_t* repository_create(resource_t* root, resource_t** patches);

void destroy_repository(repository_t* repository);

devicedef_t* repository_get_device(repository_t* repository, char* id);

u_int32_t repository_size(repository_t* repository);

#endif /* MODEL_H_ */
