/*
 * handler.c
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */
#include "parser.h"
#include "../repository.h"
#include "../collection/linkedlist.h"
#include "../collection/hashmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/encoding.h>
#include <libxml/xmlversion.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <libxml/SAX2.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>

#define ELEM_CAPABILITY BAD_CAST("capability")
#define ELEM_GROUP BAD_CAST("group")
#define ELEM_DEVICE BAD_CAST("device")

#define ATTR_ID BAD_CAST("id")
#define ATTR_NAME BAD_CAST("name")
#define ATTR_VALUE BAD_CAST("value")
#define ATTR_USER_AGENT BAD_CAST("user_agent")
#define ATTR_FALL_BACK BAD_CAST("fall_back")
#define ATTR_ACTUAL_DEVICE_ROOT BAD_CAST("actual_device_root")


struct _parse_context {
	char* current_device_id;
	char* current_device_user_agent;
	char* current_device_fall_back;
	int current_device_actual_device_root;

	char* current_group_id;

	linkedlist_t* devices;

	hashmap_t* capabilities;
	hashmap_t* groups;
};

typedef struct _parse_context parse_context;

static char* get_attribute(const xmlChar** attributes, xmlChar* name) {

	int i = 0;
	char* value = NULL;

	while(value==NULL && attributes[i]!=NULL) {
		if(xmlStrEqual(name, *(attributes+i))) {

			// check value can be casted
			value = strdup(*(attributes + (i+1)));
		}

		i++;
	}

	return value;
}


static int device_definition_equals(const void *litem, const void *ritem) {

	devicedef_t* ldev = (devicedef_t*)litem;
	devicedef_t* rdev = (devicedef_t*)ritem;

	return devicedef_cmp(ldev,rdev)==0;
}

static void* device_definition_clone(const void *item) {

	return item;
}

static void device_definition_free (void *item) {

}

static void start_document(void *ctx){

	parse_context* context = (parse_context*) ctx;

	context->devices = linkedlist_create(&device_definition_equals, &device_definition_clone, &device_definition_free);
}

static void start_group(parse_context* context, const xmlChar** attributes) {

	char* id = get_attribute(attributes, ATTR_ID);

	// FIXME check notnull

	context->current_group_id = id;
}

static void end_group(parse_context* context) {
	context->current_group_id = NULL;
}

static void start_capability(parse_context* context, const xmlChar** attributes) {

	char* name = get_attribute(attributes, ATTR_NAME);
	char* value = get_attribute(attributes, ATTR_VALUE);

	// FIXME check notnull

	hashmap_put(context->capabilities, name, value);
	hashmap_put(context->groups, name, context->current_group_id);
}

static void end_capability(parse_context* context) {

}

static void start_device(parse_context* context, const xmlChar** attributes) {

	context->current_device_id = get_attribute(attributes, ATTR_ID);
	context->current_device_user_agent = get_attribute(attributes, ATTR_USER_AGENT);
	context->current_device_fall_back = get_attribute(attributes, ATTR_FALL_BACK);
	context->current_device_actual_device_root = strcmp(get_attribute(attributes, ATTR_ACTUAL_DEVICE_ROOT),"true") == 0;

	// FIXME check

	context->capabilities = hashmap_create(&string_hash, &string_rehash, &string_cmp, &clone_item_nop, &free_item_nop, &clone_item_nop, &free_item_nop);
	context->groups = hashmap_create(&string_hash, &string_rehash, &string_cmp, &clone_item_nop, &free_item_nop, &clone_item_nop, &free_item_nop);
}

static void end_device(parse_context* context) {

	devicedef_t* devicedef = devicedef_create(context->current_device_id,
			context->current_device_user_agent,
			context->current_device_fall_back,
			context->current_device_actual_device_root,
			context->capabilities,
			context->groups);


	linkedlist_add(context->devices, devicedef);

	context->current_device_id = NULL;
	context->current_device_user_agent = NULL;
	context->current_device_fall_back = NULL;
	context->current_device_actual_device_root = 0;
	context->capabilities = NULL;
	context->groups = NULL;
}



static void start_element(void* ctx,
		const xmlChar* localname,
		const xmlChar* prefix, const xmlChar* URI, int nb_namespaces,
		const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		const xmlChar** attributes) {

	parse_context* context = (parse_context*) ctx;

	if (xmlStrEqual(localname,(xmlChar*)"device")) {
		start_device(context, attributes);
	} else if (xmlStrEqual(localname, (xmlChar*)"group")) {
		start_group(context, attributes);
	} else if (xmlStrEqual(localname, (xmlChar*)"capability")) {
		start_capability(context, attributes);
	} else {
		// ignore other
	}
}

static void end_element(void *ctx, const xmlChar *localname,
		const xmlChar *prefix, const xmlChar *URI) {

	parse_context* context = (parse_context*) ctx;

	if (xmlStrEqual(localname, (xmlChar*)"device")) {
		end_device(context);
	} else if (xmlStrEqual(localname, (xmlChar*)"group")) {
		end_group(context);
	} else if (xmlStrEqual(localname, (xmlChar*)"capability")) {
		end_capability(context);
	} else {
		// ignore other
	}
}

devicedef_t** parse_resource(char* resource) {

	parse_context context;
	context.devices = linkedlist_create(&device_definition_equals, &device_definition_clone, &device_definition_free);

	xmlSAXHandler saxHandler;
	memset(&saxHandler, 0, sizeof(saxHandler));

	xmlSAXVersion(&saxHandler, 2);
	saxHandler.startDocument = &start_document;
	saxHandler.startElementNs = &start_element;
	saxHandler.endElementNs = &end_element;

	xmlSAXUserParseFile(&saxHandler, &context, resource);

	devicedef_t** devices = malloc(sizeof(devicedef_t*) * linkedlist_size(context.devices));
	linkedlist_to_array(context.devices, (void**)devices);

	return devices;
}
