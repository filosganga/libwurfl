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

#include "wurfl.h"
#include "device.h"
#include "utils/patricia.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

int test_wurfl() {

	const char* root = "etc/wurfl.xml";
	//const char* patches[] = {};
	const char* patches[] = {"etc/web_browsers_patch.xml", NULL};

	wurfl_t* wurfl = wurfl_init(root, patches);

	device_t* device = wurfl_match(wurfl, "Nokia6601");
	if(device!=NULL) {
		fprintf(stderr, "Matched Device: %s\n", device_id(device));
	}

	wurfl_free(wurfl);

	return 0;
}



int main(int argc, char **argv) {

	test_wurfl();

	return 0;
}

