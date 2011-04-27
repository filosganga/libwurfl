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

#ifndef DEVICE_H_
#define DEVICE_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct _device_t device_t;

void device_free(device_t* device);

char* device_id(const device_t* device);

char* device_user_agent(const device_t* device);

char* device_capability(const device_t* device, const char* name);

int device_cmp(const void* ldevice, const void* rdevice);

uint32_t device_hash(const void* item);

bool device_eq(const void* litem, const void* ritem);

#endif /* DEVICE_H_ */
