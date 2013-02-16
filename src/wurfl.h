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
 * along with libWURFL.  If not, see <http://www.gnu.org/licenses/>.
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

// TODO This funcions are hidden because threadsafe is not implemented.

/**
 * This function reload the wurfl data.
 *
 * @param wurfl The wurfl to reload.
 * @param root It is the wurfl main file path.
 * @param patches It is the NULL terminated array of patches paths.
 */
//void wurfl_reload(wurfl_t* wurfl, const char* root, const char** patches);

/**
 * This function applies patches to wurfl
 *
 * @param wurfl The wurfl to patch.
 * @param patches It is the NULL terminated array of patches paths.
 */
//void wurfl_npatch(wurfl_t* wurfl, const char** patches);

/**
 * This function applies a patch to wurfl
 *
 * @param wurfl The wurfl to patch.
 * @param patch It is the patch path.
 */
//void wurfl_patch(wurfl_t* wurfl, const char* patch);

#endif /* WURFL_H_ */
