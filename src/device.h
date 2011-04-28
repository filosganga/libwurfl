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

/**
 * This function free a device obtained from wurfl.
 *
 * @param device Yhe device_t to free.
 */
void device_free(device_t* device);

/**
 * This function returns the device id.
 *
 * @param device The device to query.
 * @return The device id.
 */
char* device_id(const device_t* device);

/**
 * This function returns the device User-Agent.
 *
 * @param device The device to query.
 * @return The device User-Agent.
 */
char* device_user_agent(const device_t* device);

/**
 * This function returns a device capability by its name.
 *
 * @param device The device to query.
 * @return device capability pointed by name. NULL if the device does not
 *         contain capability
 */
char* device_capability(const device_t* device, const char* name);

/**
 * This function returns the device capabilities stored in array as sequence of name,value terminated by NULL.
 *
 * @param device The device to query
 * @param dupe The dupe function to allocate the capabilities array. If NULL by default malloc is used.
 * @return an array of name value couples followed by NULL
 */
char** device_capabilities(const device_t* device, void* (dupe)(size_t size));

/**
 * This is the device cmp function.
 *
 * @param device The device to compare
 * @param other The other device to compare
 *
 * @return cmp of the device id.
 */
int device_cmp(const void* device, const void* other);

/**
 * This compute the hash of the device.
 *
 * @param device The device to hash
 * @return computed hash
 */
uint32_t device_hash(const void* item);

/**
 * This is the device eq function.
 *
 * @param device The device to compare
 * @param other The other device to compare
 *
 * @return true if device_cmp(device, other)==0
 */
bool device_eq(const void* device, const void* other);

#endif /* DEVICE_H_ */
