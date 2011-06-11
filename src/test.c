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

#include "wurfl.h"
#include "device.h"
#include "utils/patricia.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

int test_wurfl() {

	const char* root = "../etc/wurfl.xml";
	//const char* patches[] = {};
	const char* patches[] = {"../etc/web_browsers_patch.xml", NULL};

	wurfl_t* wurfl = wurfl_init(root, patches);

	//device_t* device = wurfl_match(wurfl, "Chrome");

	device_t* device = wurfl_match(wurfl, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/534.24 (KHTML, like Gecko) Chrome/11.0.696.77 Safari/534.24");
	if(device!=NULL) {
		char** capabilities = device_capabilities(device, NULL);
		char** caps_ptr = capabilities;

		fprintf(stderr, "id: %s,\ncapabilities: {\n", device_id(device));
		while(caps_ptr!=NULL && *caps_ptr!=NULL) {
			fprintf(stderr, "    %s: %s,\n", *caps_ptr, *(caps_ptr + 1));
			caps_ptr+=2;
		}
		fprintf(stderr, "}\n");

		free(capabilities);
	}

	wurfl_free(wurfl);



	return 0;
}



int main(int argc, char **argv) {

	test_wurfl();

	return 0;
}

