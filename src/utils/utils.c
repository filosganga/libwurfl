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

#include "utils.h"

#include "error.h"

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
	// FIXME It is not a good idea
	uint32_t integer = (uint32_t)ref;

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

char* strrev(char* dst, const char* src) {

	if(src != NULL) {
		size_t src_len = strlen(src);

		int32_t i, j;
		for(i = 0, j = src_len - 1; j >= 0; i++, j--) {
			*(dst + j) = *(src + i);
		}
	}
	else {
		dst = (char*)src;
	}

	return dst;
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
