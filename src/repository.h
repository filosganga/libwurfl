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

bool repository_foreach(repository_t* repository, coll_functor_f* functor, void* functor_data);

#endif /* REPOSITORY_H_ */
