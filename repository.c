/*
 * repository.c
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */

#include "repository.h"

#include <stdlib.h>

struct _repository {
	char* version;
	hashmap_t* devices_by_id;
};

static repository* alloc_repository(){
	repository* tmp = malloc(sizeof(repository));

	if(!tmp){
		// TODO throw error
	}

	return tmp;
}

repository* create_repository(char* root, char** patches) {

	repository* rep = alloc_repository();

	//FIXME check rep

	return rep;

}


