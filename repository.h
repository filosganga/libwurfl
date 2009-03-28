/*
 * model.h
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */

#ifndef MODEL_H_
#define MODEL_H_

// DeviceDefinition *******************************************************

typedef struct _device_definiton {
	char* id;
	char* user_agent;
	char* fall_back;
	int actual_device_root;
} device_definition;

device_definition* create_device_definition(char* id, char* user_agent, char* fall_back, int actual_device_root);

void destroy_device_definition(device_definition* device);

// Hierarchy **************************************************************

typedef struct _device_hierarchy {
	int lenght;
	device_definition* devices;
} device_hierarchy;

device_hierarchy* create_hierarchy(void* devices);

void destroy_hierarchy(device_hierarchy* hierarchy);

int hierarchy_get_size(device_hierarchy* hierarchy);

char* hierarchy_get_capability(device_hierarchy* hierarchy, char* name);

device_definition* hierarchy_get_target(device_hierarchy* hierarchy);

device_definition* hierarchy_get_device(device_hierarchy* hierarchy, int index);

// Repository *************************************************************

typedef struct _repository repository;

repository* create_repository(char* root_path, char** patches_paths);

void destroy_repository(repository* rep);

#endif /* MODEL_H_ */
