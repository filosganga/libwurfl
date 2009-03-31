/*
 * utils.c
 *
 *  Created on: 28-mar-2009
 *      Author: filosganga
 */

#include "collections.h"

#include <sys/types.h>
#include <string.h>
#include <stdio.h>

static u_int32_t hash_int_impl(u_int32_t a);

void* clone_item_nop(const void *item) {
	return item;
}

void free_item_nop(void *item) {
	// Empty
}

unsigned long string_rehash(const void* item) {
   unsigned long h;
   char* string = (char*)item;

   h = 0;
   while (*string) {
      h = h * 31UL + (unsigned char) *string++;
   }
   return h;
}


unsigned long string_hash(const void* item) {
   unsigned long h;
   char* string = (char*)item;

   h = 0;
   while (*string) {
      h = h * 37UL + (unsigned char) *string++;
   }
   return h;
}

int string_cmp(const void* litem, const void *ritem) {
	char* lstring = (char*)litem;
	char* rstring = (char*)ritem;

	return strcmp(lstring, rstring);
}

int string_equals(const void* litem, const void *ritem) {
	return string_cmp(litem,ritem)==0;
}

unsigned long hash_int(const void* item) {
	u_int32_t* integer = (u_int32_t*)item;

	return hash_int_impl(*integer);
}

unsigned long rehash_int(const void* item) {
	u_int32_t* integer = (u_int32_t*)item;

	return hash_int_impl(*integer);
}

/**
 * Robert Jenkins' 32 bit integer hash function
 */
static u_int32_t hash_int_impl(u_int32_t a) {
   a = (a+0x7ed55d16) + (a<<12);
   a = (a^0xc761c23c) ^ (a>>19);
   a = (a+0x165667b1) + (a<<5);
   a = (a+0xd3a2646c) ^ (a<<9);
   a = (a+0xfd7046c5) + (a<<3);
   a = (a^0xb55a4f09) ^ (a>>16);
   return a;
}
