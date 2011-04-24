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

#include "patricia.h"

#include "utils.h"
#include "error.h"

#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>

typedef struct _trie_node_t trie_node_t;

struct _trie_node_t {
	int32_t msd;

	void *key;
	void *value;

	trie_node_t *left;
	trie_node_t *right;
};

struct _trie_t {
	trie_node_t *root;
	coll_duper_f* key_dupe;
	coll_unduper_f* key_undupe;
	void* key_dupe_xtra;
};

extern int errno;

static bool key_eq(const void *key, const void *other) {

	const char* chkey = key;
	const char* chother = other;

	if((chkey == NULL || strlen(chkey)==0) && (chother == NULL || strlen(chother)==0)){
		return true;
	}
	else if(chkey ==NULL || strlen(chkey)==0 || chother == NULL || strlen(chother)==0) {
		return false;
	}
	else {
		return strcmp(chkey, chother) == 0;
	}
}

static bool key_isset(const void *key, int32_t index) {

	const char* strkey = (char*)key;

	if(strkey == NULL) {
		return false;
	}
	else if(index >= strlen(strkey) * CHAR_BIT) {
		return false;
	}
	else {
		char mask = 1;

		int byte = index / CHAR_BIT;
		int offset = index % CHAR_BIT;

		mask = mask << offset;

		return strkey[byte] & mask;
	}
}

static trie_node_t* node_alloc(trie_t* trie, const void *key, const void *value, int32_t msd) {

	// Allocate the node
	trie_node_t* node = malloc(sizeof(trie_node_t));
	if(node==NULL) {
		error(1, errno, "error allocating new trie node");
	}

	node->key = trie->key_dupe(key, trie->key_dupe_xtra);
	node->value = value;
	node->msd = msd;

	node->left = NULL;
	node->right = NULL;

	return node;
}

static void node_free(trie_t* trie, trie_node_t* node) {
	free(node);
}

static bool node_foreach(trie_node_t* current, int32_t msd, coll_functor_f* functor, void* functor_data) {

	if(current->msd <= msd) {
		return false;
	}
	else {
		void* keyValue[2];
		keyValue[0] = current->key;
		keyValue[1] = current->value;
		bool finished = functor(keyValue, functor_data);
		finished = finished || node_foreach(current->left, current->msd, functor, functor_data);
		finished = finished || node_foreach(current->right, current->msd, functor, functor_data);
		return finished;
	}
}

void node_destroy(trie_t* trie, trie_node_t* current, int32_t msd, coll_unduper_f* undupe, void* undupe_data) {

	if(current->msd < msd) {
		node_destroy(trie, current->left, current->msd, undupe, undupe_data);
		node_destroy(trie, current->right, current->msd, undupe, undupe_data);
		node_free(trie, current);
	}
}

static trie_node_t* node_search(trie_node_t* current, const void* key, int32_t msd) {

    if (current->msd <= msd) {
    	// this is an uplink
    	return current;
    }
    else {
    	trie_node_t* next_node = key_isset(key, current->msd)?current->right:current->left;
    	return  node_search(next_node, key, current->msd);
    }
}

static trie_node_t* node_put(trie_node_t* node, trie_node_t* start, trie_node_t* parent) {

	// We have overpass the node or this is the last node
	if(start->msd >= node->msd  || start->msd <= parent->msd) {
		node->left = key_isset(node->key, node->msd)?start:node;
		node->right = key_isset(node->key, node->msd)?node:start;
		return node;
	}
	else {
		if(!key_isset(node->key, start->msd)) {
			start->left = node_put(node, start->left, start);
		}
		else {
			start->right = node_put(node, start->right, start);
		}
		return start;
	}
}

trie_t* trie_init(coll_duper_f* key_dupe, coll_unduper_f* key_undupe, void* key_dupe_xtra) {

	trie_t* trie = malloc(sizeof(trie_t));
	if(!trie) {
		error(1, errno, "error allocating new trie");
	}
	trie->key_dupe = key_dupe;
	trie->key_undupe = key_undupe;
	trie->key_dupe_xtra = key_dupe_xtra;

	if(!trie->key_dupe) {
		trie->key_dupe = &coll_nop_duper;
	}
	if(!trie->key_undupe) {
		trie->key_undupe = &coll_nop_unduper;
	}

	trie->root = node_alloc(trie, NULL, NULL, -1);
	trie->root->left = trie->root;

	return trie;
}

void trie_destroy(trie_t* trie, coll_unduper_f undupe, void* undupe_data) {
	node_destroy(trie, trie->root->left, -1, undupe, undupe_data);
	node_free(trie, trie->root);
	free(trie);
}

void trie_put(trie_t* trie, const void* key, const void* value) {

	trie_node_t* nearest = node_search(trie->root->left, key, -1);

	if(key_eq(key, nearest->key)) {
		nearest->key = key;
		nearest->value = value;
	}
	else {
		int32_t msd = 0;
		while(key_isset(key, msd)==key_isset(nearest->key, msd)) msd++;

		trie_node_t* new_node = node_alloc(trie, key, value, msd);
		trie->root->left = node_put(new_node, trie->root->left, trie->root);
	}
}

void* trie_get(trie_t* trie, const void* key) {
	trie_node_t* nearest = node_search(trie->root->left, key, -1);
    if (!key_eq(nearest->key, key)) {
        return NULL;
    }
    else {
    	return nearest->value;
    }
}

void* trie_search(trie_t* trie, const void* key) {
	trie_node_t* nearest = node_search(trie->root->left, key, -1);
    return nearest->value;
}

bool trie_foreach(trie_t* trie, coll_functor_f* functor, void* functor_data) {
	return node_foreach(trie->root->left, -1, functor, functor_data);
}

bool trie_search_foreach(trie_t* trie, const void* key, coll_functor_f* functor, void* functor_data) {
	trie_node_t* nearest = node_search(trie->root->left, key, -1);
	return node_foreach(nearest, -1, functor, functor_data);
}





