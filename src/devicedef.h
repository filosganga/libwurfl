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

#ifndef DEVICEDEF_H_
#define DEVICEDEF_H_

#include "utils/hashmap.h"

typedef struct {
	const char* id;
	const char* user_agent;
	const char* fall_back;
	bool actual_device_root;
	hashmap_t* capabilities;
} devicedef_t;

devicedef_t* devicedef_init(const char* id, const char* user_agent, const char* fallback, bool actual_device_root, hashmap_t* capabilities);

void devicedef_destroy(devicedef_t* device);

char* devicedef_id(const devicedef_t* device);

char* devicedef_user_agent(const devicedef_t* device);

char* devicedef_fallback(const devicedef_t* device);

devicedef_t* devicedef_patch(devicedef_t* patching, const devicedef_t* patcher);

int devicedef_cmp(const void* left, const void* right);

uint32_t devicedef_hash(const void* item);

bool devicedef_eq(const void* litem, const void* ritem);

#endif /* DEVICEDEF_H_ */
