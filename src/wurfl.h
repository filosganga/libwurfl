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
 * @param root It is the wurfl main file path.
 * @param patches It is the NULL terminated array of patches paths.
 *
 * @return intialized wurfl.
 */
wurfl_t* wurfl_init(const char* root, const char** patches);

/**
 * This function destroy the wurfl
 * @param wufl The wurfl to free.
 */
void wurfl_free(wurfl_t* wurfl);

/**
 * This is the match function
 *
 * @param wurfl The wurfl used to match.
 * @param user_agent The user_agent to match. It must be != NULL
 *
 * @return the device_t matched from user_agent.
 */
device_t* wurfl_match(const wurfl_t* wurfl, const char* user_agent);

/**
 * This function reload the wurfl data.
 *
 * @param wurfl The wurfl to reload.
 * @param root It is the wurfl main file path.
 * @param patches It is the NULL terminated array of patches paths.
 */
void wurfl_reload(wurfl_t* wurfl, const char* root, const char** patches);

/**
 * This function applies patches to wurfl
 *
 * @param wurfl The wurfl to patch.
 * @param patches It is the NULL terminated array of patches paths.
 */
void wurfl_npatch(wurfl_t* wurfl, const char** patches);

/**
 * This function applies a patch to wurfl
 *
 * @param wurfl The wurfl to patch.
 * @param patch It is the patch path.
 */
void wurfl_patch(wurfl_t* wurfl, const char* patch);

/**
 * This function return the wurfl devices size.
 *
 * @param wurfl The wurfl to query.
 * @return The number of instantiated devices in wurfl.
 */
size_t wurfl_size(wurfl_t* wurfl);

/**
 * This function return the wurfl capabilities size.
 *
 * @param wurfl The wurfl to query.
 * @return The number of known capabilities.
 */
size_t wurfl_capabilities_size(wurfl_t* wurfl);

#endif /* WURFL_H_ */
