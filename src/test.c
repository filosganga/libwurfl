/*
 * test.c
 *
 *  Created on: 19/apr/2011
 *      Author: filippodeluca
 */

#include "wurfl.h"
#include "device.h"


#include <stdio.h>
#include <string.h>
#include <libxml/xmlstring.h>


//int test_strings() {
//
//	const unsigned char* key = "aaa";
//	xmlChar* xml = xmlCharStrdup(key);
//
//	int size = strlen(key) * 8;
//	int bit = size;
//
//	fprintf(stderr, "key=");
//	for(bit=size; bit>0; bit--) {
//		fprintf(stderr, "%d ",(key[bit >> 3] & (1 << (bit & 7))));
//	}
//	fprintf(stderr, "\n");
//
//	fprintf(stderr, "xml=");
//	for(bit=size; bit>0; bit--) {
//		fprintf(stderr, "%d ",(key[bit >> 3] & (1 << (bit & 7))));
//	}
//	fprintf(stderr, "\n");
//
//	return 0;
//
//}
//
int test_wurfl() {

	//const char* main = argv[1];
	//const char* patches[] = {};
	//const char* patches[] = {"etc/web_browsers_patch.xml", NULL};

	const char* main = "../etc/root.xml";
	const char* patches[] = {};

	wurfl_t* wurfl = wurfl_init(main, patches);

	device_t* device = wurfl_match(wurfl, "DEVICE A A");
	if(device!=NULL) {
		fprintf(stderr, "Matched Device: %s\n", device_id(device));
	}

	return 0;
}
//
//int test_repository() {
//
//	const char* main = "../etc/root.xml";
//	const char* patches[] = {};
//
//	repository_t* repo = repository_create(main, patches);
//
//	fprintf(stderr, "Before foreach\n");
//	repository_foreach(repo, &print_device, NULL);
//
//	return 0;
//}
//
//int test_resource() {
//
//	const char* main_pth = "../etc/root.xml";
//	const char* patches[] = {};
//
//	resource_data_t data;
//
//	hashtable_options_t strings_opts = {1000, 1.5f};
//	hashtable_t* strings = hashtable_create(&string_eq, &string_hash, &strings_opts);
//
//	hashmap_options_t devices_opts = {10000, 1.5f};
//	data.devices = hashmap_create(&string_eq, &string_hash, &devices_opts);
//	resource_parse(&data, main_pth, strings);
//
//
//	return 0;
//}

int main(int argc, char **argv) {

	//test_strings();

	// test_repository();

	// test_resource();

	test_wurfl();

	return 0;
}

