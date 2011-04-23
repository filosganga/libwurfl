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

#ifndef DEVICE_IMPL_H_
#define DEVICE_IMPL_H_

#include "device.h"

#include "repository.h"
#include "devicedef.h"

struct _device_t {
	devicedef_t* root;
	repository_t* repository;
};

device_t* device_create(repository_t* repository, const devicedef_t* devicedef);

#endif /* DEVICE_IMPL_H_ */
