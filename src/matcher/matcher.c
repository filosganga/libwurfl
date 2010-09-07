/*
 * matcher.c
 *
 *  Created on: 27/ago/2010
 *      Author: Filippo De Luca
 */

#include "device-impl.h"

#include <string.h>

typedef struct _trie_node_t trie_node_t;

typedef struct _trie_t trie_t;

struct _trie_node_t {
	const char* key;
	device_t* device;
	trie_node_t* parent;
	trie_node_t* first_child;
	trie_node_t* next_sibling;
};

struct _trie_t {
	trie_node_t* root_node;
};

void trie_put(trie_t* trie, const char* key, device_t* device) {

}

trie_node_t* create_parent_node(trie_node_t* node) {

	trie_node_t* parent = malloc(sizeof(trie_node_t));

	// TODO

}

void trie_node_put(trie_node_t* node, device_t* device, size_t key_offset) {

//	// TODO check the string matches
//	size_t key_len = strlen(node->key);
//	size_t user_agent_len = strlen(*(device->user_agent + key_offset));
//
//	if(user_agent_len < key_len) {
//		// TODO create supernode
//	}
//	else {
//
//		// if device->user_agent contains key
//		//find subnode
//
//
//		// else
//		// create subnode with one child
//	}
//
//
//	int cmp = strncmp(key, device->id + key_offset, key_len);
//	if(cmp==0) {
//		// find child
//		size_t new_offset = key_offset + key_len;
//
//		if(node->first_child==NULL) {
//			// TODO add child
//		}
//		else {
//
//			trie_node_t* candidate = node->first_child;
//		}
//		/* TODO
//		 *
//		 * while id+new_offset.startWith(candidate) {
//		 *   candidate = candidate.nextSibling;
//		 * }
//		 *
//		 */
//
//		if(false /*found*/) {
//			trie_node_put(candidate, device, new_offset);
//		}
//		else {
//			// TODO add node
//			trie_node_t new_node = malloc(sizeof(trie_node_t));
//			new_node->device = device;
//			new_key_len = strlen(device->id) - new_offset;
//			new_node->key = malloc(sizeof(char) * (new_key_len + 1));
//			strncpy(new_node->key, *(device->id + new_offset), new_key_len);
//			new_node->first_child = NULL;
//			new_node->next_sibling = NULL;
//
//			candidate->next_sibling = new_node;
//		}
//
//
//	}
//	else {
//		// TODO split this node
//	}

}
