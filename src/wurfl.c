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

#include "wurfl.h"

#include "repository.h"
#include "matcher.h"
#include "device-impl.h"
#include "devicedef.h"
#include "utils/utils.h"
#include "utils/functors.h"
#include "utils/error.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

extern int errno;

struct _wurfl_t {
	repository_t* repository;
	matcher_t* matcher;
};

wurfl_t* wurfl_init(const char* root, const char** patches) {

	wurfl_t* wurfl = malloc(sizeof(wurfl_t));
	if(wurfl==NULL) {
		error(1,errno,"error allocating memory to wurfl");
	}

	wurfl->repository = repository_create(root, patches);
	wurfl->matcher = matcher_init(wurfl->repository);

	return wurfl;
}

void wurfl_destroy(wurfl_t* wurfl) {

	matcher_destroy(wurfl->matcher);
	repository_destroy(wurfl->repository);

	free(wurfl);
}

device_t* wurfl_match(const wurfl_t* wurfl, const char* user_agent) {

	devicedef_t* matched = matcher_match(wurfl->matcher, user_agent);

	return device_create(wurfl->repository, matched);
}


