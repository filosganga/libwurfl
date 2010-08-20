/*
 * matcher.c
 *
 *  Created on: Apr 29, 2010
 *      Author: filosganga
 */

#include "matcher.h"

#include "utils/collection/hashset.h"

#include <datrie/trie.h>
#include <stdio.h>
#include <strings.h>
#include <assert.h>

struct matcher_t_ {
	Trie* trie;
};

static Bool enumerate(const AlphaChar *key, TrieData key_data, void *user_data) {

	devicedef_t* devicedef = key_data;

	fprintf(stderr,"key: %s\n", devicedef_get_id(devicedef));
	return false;
}

static int add_to_trie(const void* item, void* data) {

	devicedef_t* device = (devicedef_t*)item;
	Trie* trie = (Trie*)data;

	assert(device!=NULL);
	assert(trie!=NULL);

	const char* user_agent = devicedef_get_user_agent(device);

	if(user_agent!=NULL) {
		trie_store(trie, user_agent, device);
	}

	return 0;
}

matcher_t* matcher_create(repository_t* repository) {

	matcher_t* matcher = malloc(sizeof(matcher_t));

	AlphaMap* alpha_map = alpha_map_new();
	//alpha_map_add_range(alpha_map, " ", "~");
	matcher->trie = trie_new(alpha_map);

	coll_functor_t functor;
	functor.data = matcher->trie;
	functor.functor = &add_to_trie;

	repository_foreach(repository, &functor);

	trie_enumerate(matcher->trie, &enumerate, NULL);

	return matcher;
}

const char* matcher_match(matcher_t* matcher, const char* user_agent) {

	char* result = NULL;

	size_t lenght = strlen(user_agent);
	char* reduced = malloc(sizeof(char)*(lenght+1));
	strcpy(reduced, user_agent);

	devicedef_t* device=NULL;
	while(lenght>0 && !trie_retrieve(matcher->trie, reduced, &device)) {
		lenght--;
		*(reduced + lenght)='\0';
	}

	if(device !=NULL) {
		result = devicedef_get_id(device);
	}

	return result;
}
