/*
 * repository.c
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */

#include "repository.h"


#include <stdlib.h>

struct _repository_t {
	char* version;
	hashmap_t* devices_by_id;
};

static repository_t* alloc_repository(){
	repository_t* tmp = malloc(sizeof(repository_t));

	if(!tmp){
		// TODO throw error
	}

	return tmp;
}

repository_t* repository_create(resource_t* root, resource_t** patches) {

	repository_t* rep = alloc_repository();

	resource_data_t* root_data = resource_parse(root);
	hashset_t* devices = root_data->devices;
	// TODO copy devices? yes!
	resource_data_destroy(root_data);

	return rep;

}


