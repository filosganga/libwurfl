/*
 * hashset-test.c
 *
 *  Created on: Nov 24, 2009
 *      Author: filosganga
 */

#include <CUnit/CUnit.h>

#include "hashset-test.h"
#include "../../main/collection/hashset.h"

void hashset_create_should_return_not_null() {

	hashset_t* set = hashset_create(&string_hash, &string_rehash, &string_equals);

	CU_ASSERT_PTR_NOT_NULL_FATAL(set);

}

void hashset_to_array_should_return_right_size() {

	hashset_t* set = hashset_create(&string_hash, &string_rehash, &string_equals);

	seedMT(1);

	for (int i = 0; i < 1000; i++) {

		char* string = malloc(sizeof(char) * 17);

		for (int char_index = 0; char_index < 16; char_index++) {
			int c = (randomMT() % 57) + 65;
			*(string + char_index) = c;
		}

		*(string + 16) = '\0';
		hashset_add(set, string);
	}

	char** array = NULL;

	array = hashset_to_array(set);

	CU_ASSERT_PTR_NOT_NULL(array[999])
}
