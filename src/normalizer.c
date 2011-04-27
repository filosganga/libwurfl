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

#include "normalizer.h"

#include "utils/linkedlist.h"
#include "utils/error.h"
#include "utils/utils.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>
#include <errno.h>
#include <assert.h>

extern int errno;

struct _normalizer_t {
	regex_t* vdfnsn_regex;
	linkedlist_t* handlers;
};

typedef struct {
	normalizer_t* normalizer;
	char* dst;
	char* src;
} handler_xtra_t;

typedef bool (handler_f)(normalizer_t* normalizer, char* dst, const char* src);

bool apply_handler(void* item, void* xtra);

/**
 * Removes the "(via babelfish.yahoo.com\\)" String.
 */
static bool normalize_babelfish(normalizer_t* normalizer, char* dst, const char* src) {

	strcpy(dst, src);

	char* pos = strstr(dst, " (via babelfish.yahoo");
	if(pos) {
		*pos = '\0';
		return true;
	}
	else {
		return false;
	}

}

/**
 * Strips out the trailing "UP.Link*" from the target user-agent.
 */
static bool normalize_uplink(normalizer_t* normalizer, char* dst, const char* src) {

	strcpy(dst, src);

	char* pos = strstr(dst, " UP.Link)");
	if(pos) {
		*pos = '\0';
		return true;
	}
	else {
		return false;
	}

}

/**
 * "\\s*Mozilla/4\\.0 \\(YesWAP mobile phone proxy\\)"
 */
static bool normalize_yeswap(normalizer_t* normalizer, char* dst, const char* src) {

	strcpy(dst, src);

	char* pos = strstr(dst, " Mozilla/4.0 (YesWAP");
	if(pos) {
		*pos = '\0';
		return true;
	}
	else {
		return false;
	}
}

/**
 * Normalize the "/SNnnnnnnnnnnnnnnnn" String.
 */
static bool normalize_vodafonesn(normalizer_t* normalizer, char* dst, const char* src) {

	strcpy(dst, src);

	regmatch_t groups[2];
	int matches = regexec(normalizer->vdfnsn_regex, dst, (size_t)2, groups, REG_NOTBOL | REG_NOTEOL);
	if(matches == 0){

		regmatch_t group = groups[1];
		size_t sn_len = (group.rm_eo - group.rm_so);

		uint32_t i;
		char *sn_idx = dst + group.rm_so;
		for(i=0; i<sn_len; i++) {
			*(sn_idx + i) = 'X';
		}

		return true;
	}
	else if(matches != REG_NOMATCH) {
		error(0,0, "regex error: %d", matches);
		return false;
	}
	else {
		return false;
	}
}

normalizer_t* normalizer_init() {

	normalizer_t* normalizer = malloc(sizeof(normalizer_t));
	if(!normalizer) {
		error(1, errno, "error allocating normalizer");
	}

	normalizer->vdfnsn_regex = malloc(sizeof(regex_t));
	if(!normalizer->vdfnsn_regex) {
		error(1, errno, "error allocatng memory for vdfnsn_regex");
	}

	if(regcomp(normalizer->vdfnsn_regex, "/SN([0-9]+)[ ]*", REG_EXTENDED)!=0) {
		assert(false);
	}

	normalizer->handlers = linkedlist_create(&ref_eq);
	linkedlist_add(normalizer->handlers, &normalize_yeswap);
	linkedlist_add(normalizer->handlers, &normalize_babelfish);
	linkedlist_add(normalizer->handlers, &normalize_uplink);
	linkedlist_add(normalizer->handlers, &normalize_vodafonesn);

	return normalizer;
}

void normalizer_free(normalizer_t* normalizer) {

	linkedlist_destroy(normalizer->handlers, &coll_nop_unduper, NULL);
	regfree(normalizer->vdfnsn_regex);
	free(normalizer);

}

void normalizer_apply(normalizer_t* normalizer, char* dst, const char* src) {

	handler_xtra_t handler_xtra;
	handler_xtra.normalizer = normalizer;

	handler_xtra.src = malloc(sizeof(char) * (strlen(src) + 1));
	if(!handler_xtra.src) {
		error(1, errno, "error allocating src string");
	}
	handler_xtra.dst = malloc(sizeof(char) * 8 * 1024);
	if(!handler_xtra.dst) {
		error(1, errno, "error allocating dst string");
	}
	strcpy(handler_xtra.src, src);

	linkedlist_foreach(normalizer->handlers, &apply_handler, &handler_xtra);
	strcpy(dst, handler_xtra.dst);

	free(handler_xtra.src);
	free(handler_xtra.dst);
}

bool apply_handler(void* item, void* xtra) {
	handler_f* handler = (handler_f*)item;
	handler_xtra_t* handler_xtra = (handler_xtra_t*)xtra;

	if(handler(handler_xtra->normalizer, handler_xtra->dst, handler_xtra->src)) {

		// SWAP the strings
		char* tmp = handler_xtra->src;
		memset(tmp, '\0', strlen(tmp) * sizeof(char));
		handler_xtra->src = handler_xtra->dst;
		handler_xtra->dst = tmp;
	}

	// Apply all not chain of responsibility
	return false;

}


