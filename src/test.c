/*
 * test.c
 *
 *  Created on: 30-mar-2009
 *      Author: filosganga
 */

#include "collection/collections.h"
#include "collection/hashset.h"
#include "collection/cokusmt.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

#include "resource/resource.h"
#include "resource/repository.h"

typedef struct _show_data_t {
	int index;
} show_data_t;

static int show(void* item, void* data) {

	char* string = item;
	show_data_t* show_data = data;

	printf("%s at %d\n", string, show_data->index);
	show_data->index++;

	return 0;
}

static int show_devicedef(void* item, void* data) {

	devicedef_t* devicedef = item;
	show_data_t* show_data = data;

	printf("%s at %d\n", devicedef_get_id(devicedef), show_data->index);
	show_data->index++;

	return 0;
}

static void test_hashset() {

	hashset_t* set = hashset_create(&string_hash, &string_rehash, &string_equals);

	seedMT(1);

	for(int i=0; i<6000; i++) {

		char* string = malloc(sizeof(char) * 17);

		for(int char_index=0; char_index<16; char_index++) {
			char* c = (randomMT() % 57) + 65 ;
			*(string + char_index) = c;
		}

		*(string + 16) = '\0';
		hashset_add(set, string);
	}

	printf("hashset.size: %d", hashset_size(set));

	char** array = hashset_to_array(set);

	show_data_t show_data;
	show_data.index = 0;

	hashset_foreach(set,&show, &show_data);

}

static void test_repository() {

	resource_t* resource = resource_create("/home/filosganga/documents/development/tmp/wurfl.xml");
	repository_t* repository = repository_create(resource, NULL);

	show_data_t show_data;
	show_data.index = 0;

	devicedef_t* generic = repository_get_device(repository, "generic");
	printf("Generic: %s\n",devicedef_get_id(generic));
}

int main(int argc, char** args) {

	test_repository();

	return 0;
}
