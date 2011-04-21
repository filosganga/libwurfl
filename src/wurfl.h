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
#ifndef WURFL_H_
#define WURFL_H_

#include "device.h"

#include <wchar.h>

typedef struct _request_t {
	const char* user_agent;
} request_t;

typedef struct _wurfl_t wurfl_t;

wurfl_t* wurfl_init(const char* root, const char** patches);

void wurfl_destroy(wurfl_t* wurfl);

device_t* wurfl_match(const wurfl_t* wurfl, const wchar_t* user_agent, const char* encoding);

#endif /* WURFL_H_ */
