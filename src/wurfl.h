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

#ifndef WURFL_H_
#define WURFL_H_

#include "device.h"

typedef struct _request_t {
	const char* user_agent;
} request_t;

typedef struct _wurfl_t wurfl_t;

/**
 * This function init the wurfl with the given main file and patches
 */
wurfl_t* wurfl_init(const char* root, const char** patches);

/**
 * This function destroy the wurfl
 */
void wurfl_destroy(wurfl_t* wurfl);

/**
 * This is the match function
 * @user_agent must be != NULL
 */
device_t* wurfl_match(const wurfl_t* wurfl, const char* user_agent);

#endif /* WURFL_H_ */
