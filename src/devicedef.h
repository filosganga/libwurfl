/* Copyright (C) 2011 Fantayeneh Asres Gizaw, Filippo De Luca
 *  
 * This file is part of libWURFL.
 *
 * libWURFL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or 
 * any later version.
 *
 * libWURFL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */
 
/* Written by Filippo De Luca <me@filippodeluca.com>.  */

#ifndef DEVICEDEF_H_
#define DEVICEDEF_H_

#include "utils/hashmap.h"

typedef struct _devicedef_t devicedef_t;

struct _devicedef_t {
	char* id;
	char* user_agent;
	char* fall_back;
	bool actual_device_root;
	hashmap_t* capabilities;
};

devicedef_t* devicedef_init(char* id, char* user_agent, char* fallback, bool actual_device_root, hashmap_t* capabilities);

void devicedef_free(devicedef_t* device);

char* devicedef_id(const devicedef_t* device);

char* devicedef_user_agent(const devicedef_t* device);

char* devicedef_fallback(const devicedef_t* device);

devicedef_t* devicedef_patch(devicedef_t* patching, const devicedef_t* patcher);

int devicedef_cmp(const void* left, const void* right);

uint32_t devicedef_hash(const void* item);

bool devicedef_eq(const void* litem, const void* ritem);

#endif /* DEVICEDEF_H_ */
