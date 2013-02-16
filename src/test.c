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
#include <check.h>

extern int errno;

typedef struct {
	const char* user_agent;
	const char* ids;
} test_entry_t;

const char* root = "../etc/wurfl.xml";
const char* patches[] = {NULL};

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

	wurfl_t* wurfl = wurfl_init(root, patches);

	linkedlist_foreach(test_entries, test_user_agent, wurfl);

	wurfl_free(wurfl);
}

START_TEST(capabilities) {

	const char* user_agent = "Mozilla/5.0 (Linux; U; Android 2.2; en-us; Nexus One Build/FRF91) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1";

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
	
	fail_unless(device!=NULL, NULL);

	device_free(device);
	wurfl_free(wurfl);
}
END_TEST

START_TEST(normalizers) {

	const char* expected = "Mozilla/5.0 (Linux; U; Android 2.2; xx-xx; Nexus One Build/FRF91) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1";

	const char* language_input = "Mozilla/5.0 (Linux; U; Android 2.2; en-us; Nexus One Build/FRF91) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1";
	const char language_output[1024];
	memset(language_output, '\0', sizeof(char) * 1024);

	normalizer_t* normalizer = normalizer_init();

	normalizer_apply(normalizer, language_output, language_input);
	fprintf(stderr, "Normalized User-Agent:\n %s\n", language_output);
	
	fail_unless(strcmp(language_output, expected)==0, NULL);

	normalizer_free(normalizer);
}
END_TEST

static int test_match(const char* user_agent, const char* expected_id) {
	
	const char* root = "../etc/wurfl.xml";
	const char* patches[] = {NULL};

	wurfl_t* wurfl = wurfl_init(root, patches);
	
	device_t* device = wurfl_match(wurfl, user_agent);
	
	int result = strcmp(device_id(device), expected_id);
	if(result!=0) {
		fprintf(stderr, "BAd match:\n\tuser-agent: %s\n\tmatched to: %s\n\twith user-agent: %s\n\n",user_agent, device_id(device), device_user_agent(device));	
	}
	
	device_free(device);
	wurfl_free(wurfl);
	
	return result;
}

START_TEST(matching) {
	
	const char* user_agent = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_2) AppleWebKit/536.26.14 (KHTML, like Gecko) Version/6.0.1 Safari/536.26.14";
	const char* expected = "safari_6_0_mac";

	fail_unless(test_match(user_agent, expected)==0, NULL);	
	
}
END_TEST

Suite* wurfl_suite (void) {
		
	TCase* tc_core = tcase_create("Core");
	tcase_add_test(tc_core, capabilities);
	tcase_add_test(tc_core, normalizers);
	tcase_add_test(tc_core, matching);
	
	Suite* suite = suite_create("libwurfl");
	suite_add_tcase(suite, tc_core);
	
	return suite;
}

int main(int argc, char** argv) {
	
	int number_failed;
	
	Suite* suite = wurfl_suite();
	SRunner* runner = srunner_create(suite);
	srunner_run_all(runner, CK_NORMAL);
	number_failed = srunner_ntests_failed (runner);
	srunner_free(runner);
	
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

