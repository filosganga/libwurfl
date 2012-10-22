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
#include "normalizer.h"
#include "utils/utils.h"
#include "utils/patricia.h"
#include "utils/linkedlist.h"


#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <errno.h>

extern int errno;

typedef struct {
	const char* user_agent;
	const char* ids;
} test_entry_t;


bool test_user_agent(const void* item, void* data) {
	wurfl_t* wurfl = (wurfl_t*)data;
	test_entry_t* entry = (test_entry_t*) item;

	device_t* device = wurfl_match(wurfl, entry->user_agent);
	if(device!=NULL) {
		if(strcmp(device_id(device), entry->ids)!=0) {
			fprintf(stderr, "User-Agent:\n %s\nMatched as: %s instead of: %s\n", entry->user_agent, device_id(device), entry->ids);
			return true;
		}
		return false;
	}
	else {
		return true;
	}
}


int test_user_agents(linkedlist_t* test_entries) {

	const char* root = "../etc/wurfl.xml";
	const char* patches[] = {"../etc/web_browsers_patch.xml", NULL};

	wurfl_t* wurfl = wurfl_init(root, patches);

	linkedlist_foreach(test_entries, test_user_agent, wurfl);

	wurfl_free(wurfl);
}

int test_wurfl() {

	const char* user_agent = "Mozilla/5.0 (Linux; U; Android 2.2; en-us; Nexus One Build/FRF91) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1";
	//const char* user_agent = "Chrome";

	const char* root = "../etc/wurfl.xml";
	//const char* patches[] = {};
	const char* patches[] = {"../etc/web_browsers_patch.xml", NULL};

	wurfl_t* wurfl = wurfl_init(root, patches);

	device_t* device = wurfl_match(wurfl, user_agent);
	if(device!=NULL) {
		char** capabilities = device_capabilities(device, NULL);
		char** caps_ptr = capabilities;

		fprintf(stderr, "id: %s,\ncapabilities: {\n", device_id(device));
		while(caps_ptr!=NULL && *caps_ptr!=NULL) {
			fprintf(stderr, "    %s = %s,\n", *caps_ptr, *(caps_ptr + 1));
			caps_ptr+=2;
		}
		fprintf(stderr, "}\n");

		free(capabilities);
	}

	wurfl_free(wurfl);

	return 0;
}

int test_normalizers() {

	const char* language_input = "Mozilla/5.0 (Linux; U; Android 2.2; en-us; Nexus One Build/FRF91) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1";
	const char language_output[1024];
	memset(language_output, '\0', sizeof(char) * 1024);

	normalizer_t* normalizer = normalizer_init();

	normalizer_apply(normalizer, language_output, language_input);
	fprintf(stderr, "Normalized User-Agent:\n %s\n", language_output);
	if(strcmp(language_output, "Mozilla/5.0 (Linux; U; Android 2.2; xx-xx; Nexus One Build/FRF91) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1")!=0) {
		assert(false);
	}

	normalizer_free(normalizer);

	return 0;
}


int main(int argc, char **argv) {

	test_normalizers();
	test_wurfl();
	//test_user_agents();

	return 0;
}

