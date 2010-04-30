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
#include <assert.h>

struct matcher_t_ {
	Trie* trie;
};

static bool enumerate(const AlphaChar *key, TrieData key_data, void *user_data) {

	fprintf(stderr,"key: %s\n", key);
	return false;
}

static int add_to_trie(const devicedef_t* device, const Trie* trie) {

	assert(device != NULL);
	assert(trie!=NULL);

	if(devicedef_get_user_agent(device)!=NULL) {
		if(!trie_store(trie, devicedef_get_user_agent(device), NULL)) {
			fprintf("Device: %s with user-agent: %s not stored\n", devicedef_get_id(device), devicedef_get_user_agent(device));
		}
	}

	return 0;
}

matcher_t* matcher_create(repository_t* repository) {

	matcher_t* matcher = malloc(sizeof(matcher_t));

	AlphaMap* alpha_map = alpha_map_new();
	//alpha_map_add_range(alpha_map, " ", "~");
	matcher->trie = trie_new(alpha_map);

	hashset_t* devices = repository_get_devices(repository);

	coll_functor_t functor;
	functor.data = matcher->trie;
	functor.functor = &add_to_trie;

	hashset_foreach(devices, &functor);

	trie_enumerate(matcher->trie, &enumerate, NULL);

	return matcher;
}

const char* matcher_match(matcher_t* matcher, const char* user_agent) {

	char* result = NULL;

	devicedef_t* device = NULL;
	if(trie_retrieve(matcher->trie, user_agent, device)){
		result = devicedef_get_id(device);
	}
	else {
		result = "NOT FOUND";
	}

	return result;
}
