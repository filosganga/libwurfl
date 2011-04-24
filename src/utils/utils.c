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

#include "utils.h"

#include "error.h"
#include "../../config.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

extern int errno;

static uint32_t hash_int_impl(uint32_t a);

void* coll_nop_duper(const void* item, const void* xtra) {
	return item;
}

void coll_nop_unduper(void* item, const void* xtra) {

}

void coll_default_unduper(void* item, const void* xtra) {
	free(item);
}

// Primitive utility functions ********************************************

uint32_t string_hash(const void* item) {
   unsigned long h;
   char* string = (char*)item;

   h = 0;
   while (*string) {
      h = h * 37UL + (char) *string++;
   }
   return h;
}

int string_cmp(const void* litem, const void *ritem) {
	char* lstring = (char*)litem;
	char* rstring = (char*)ritem;

	return strcmp(lstring, rstring);
}

bool string_eq(const void* litem, const void *ritem) {
	char* lstring = (char*)litem;
	char* rstring = (char*)ritem;

	return strcmp(lstring, rstring)==0;
}


uint32_t ref_hash(const void* ref) {
	uint32_t* integer = (uint32_t*)ref;

	return hash_int_impl(integer);
}

int ref_cmp(const void* litem, const void *ritem) {
	return litem - ritem;
}

bool ref_eq(const void* litem, const void *ritem) {
	return litem == ritem;
}

uint32_t int_hash(const void* item) {
	uint32_t* integer = (uint32_t*)item;

	return hash_int_impl(*integer);
}

int int_cmp(const void* litem, const void *ritem) {
	uint32_t* lint = (uint32_t*)litem;
	uint32_t* rint = (uint32_t*)ritem;

	return *lint - *rint;
}

bool int_eq(const void* litem, const void *ritem) {
	uint32_t* lint = (uint32_t*)litem;
	uint32_t* rint = (uint32_t*)ritem;

	return *lint==*rint;
}

char* strrev(const char *s) {

	char *t;

	if(s != NULL) {
		size_t s_len = strlen(s);
		t = malloc(sizeof(char) * (s_len + 1));
		if(!t) {
			error(1, errno, "error allocating string for rverse");
		}

		uint32_t i, j;
		for(i = 0, j = s_len - 1; j >= 0; i++, j--) {
			*(t + j) = *(s + i);
		}
	}
	else {
		t = (char*)s;
	}

	return t;
}

/**
 * Robert Jenkins' 32 bit integer hash function
 */
static uint32_t hash_int_impl(uint32_t a) {
   a = (a+0x7ed55d16) + (a<<12);
   a = (a^0xc761c23c) ^ (a>>19);
   a = (a+0x165667b1) + (a<<5);
   a = (a+0xd3a2646c) ^ (a<<9);
   a = (a+0xfd7046c5) + (a<<3);
   a = (a^0xb55a4f09) ^ (a>>16);
   return a;
}
