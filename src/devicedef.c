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

#include "devicedef.h"

#include "utils/utils.h"
#include "utils/error.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

extern int errno;

devicedef_t* devicedef_init(char* id,
		char* user_agent,
		char* fallback,
		bool actual_device_root,
		hashmap_t* capabilities) {

	devicedef_t* device = malloc(sizeof(devicedef_t));
	if(device==NULL) {
		error(1, errno, "error allocating device");
	}

	device->id = id;
	device->user_agent = user_agent;
	device->fall_back = fallback;
	device->capabilities = capabilities;

	return device;
}

void devicedef_free(devicedef_t* device) {

	hashmap_free(device->capabilities, &coll_default_unduper, NULL);
	free(device);
}

devicedef_t* devicedef_patch(devicedef_t* patching, const devicedef_t* patcher) {

	if(patcher->user_agent != NULL) {
		free(patching->user_agent);
		patching->user_agent = patcher->user_agent;
	}

	if(patcher->fall_back != NULL) {
		free(patcher->fall_back);
		patching->fall_back = patcher->fall_back;
	}

	patching->actual_device_root = patcher->actual_device_root;

	hashmap_putall(patching->capabilities, patcher->capabilities);
	hashmap_free(patcher->capabilities, NULL, NULL);

	return patching;
}

char* devicedef_id(const devicedef_t* device) {
	return device->id;
}

char* devicedef_user_agent(const devicedef_t* device) {
	return device->user_agent;
}

char* devicedef_fallback(const devicedef_t* device) {
	return device->fall_back;
}

int devicedef_cmp(const void* litem, const void* ritem) {

	devicedef_t* ldevicedef = (devicedef_t*)litem;
	devicedef_t* rdevicedef = (devicedef_t*)ritem;

	return strcmp(ldevicedef->id, rdevicedef->id);
}

uint32_t devicedef_hash(const void* item) {

	devicedef_t* devicedef = (devicedef_t*)item;

	return string_hash(devicedef->id);
}

bool devicedef_eq(const void* litem, const void* ritem) {

	devicedef_t* ldevicedef = (devicedef_t*)litem;
	devicedef_t* rdevicedef = (devicedef_t*)ritem;

	return strcmp(ldevicedef->id, rdevicedef->id)==0;
}
