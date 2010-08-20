/*
 * Copyright 2010 Filippo De Luca
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

/*
 * wurfl.h
 *
 *  Created on: 23-mar-2009
 *      Author: Filippo De Luca
 */
#include "device.h"

#include "utils/memory/allocator.h"

#ifndef WURFL_H_
#define WURFL_H_

const char* program_name = "cwurfl";

device_t* get_device_from_request(void* request);

device_t* get_device_from_user_agent(char* user_agent);

#endif /* WURFL_H_ */
