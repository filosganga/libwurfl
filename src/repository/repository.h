/*
 * repository.h
 *
 *  Created on: 23-mar-2009
 *      Author: Filippo De Luca
 */

#ifndef REPOSITORY_H_
#define REPOSITORY_H_

#include "resource/resource.h"
#include "devicedef.h"
#include "hierarchy.h"

#include "utils/collection/hashmap.h"
#include "utils/collection/linkedlist.h"

#include <stdint.h>

typedef struct _repository_t repository_t;

repository_t* repository_create(resource_t* root, resource_t** patches);

void destroy_repository(repository_t* repository);

devicedef_t* repository_get_device(repository_t* repository, char* id);

hashset_t* repository_get_devices(repository_t* repository);

uint32_t repository_size(repository_t* repository);


#endif /* REPOSITORY_H_ */
