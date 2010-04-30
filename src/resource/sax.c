/*
 * handler.c
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */
#include "resource-impl.h"
#include "repository/repository.h"
#include "repository/devicedef-impl.h"
#include "repository/hierarchy-impl.h"

#include "utils/collection/collections.h"
#include "utils/collection/hashmap.h"
#include "utils/collection/hashset.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

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

typedef struct {
	uint32_t size;

	bool current_device_actual_device_root;
	char* current_device_id;
	char* current_device_user_agent;
	char* current_device_fall_back;

	hashset_t* devices;
	hashmap_t* capabilities;

	hashset_t* strings;
} parse_context_t;


static char* get_attribute(parse_context_t* context, const xmlChar** attributes, xmlChar* name) {

	int i = 0;
	char* value = NULL;

	while(attributes[i]!=NULL && !xmlStrEqual(name, attributes[i])) i+=2;

	if(attributes[i]!=NULL) {
		value = (char*)xmlStrdup(attributes[i+1]);
	}

	return value;
}


static void start_document(void *ctx){

	parse_context_t* context = (parse_context_t*) ctx;
	context->size = 0;
	context->capabilities=NULL;
	context->current_device_actual_device_root = false;
	context->current_device_fall_back = NULL;
	context->current_device_id = NULL;
	context->current_device_user_agent = NULL;

	context->devices = hashset_create(&devicedef_hash, &devicedef_rehash, &devicedef_cmp);
}

static void end_document(void *ctx){

	parse_context_t* context = (parse_context_t*) ctx;
	context->size = 0;
}

static int string_equals(const char* left, char* right) {
	return strcmp(left,right)==0;
}

static void start_capability(parse_context_t* context, const xmlChar** attributes) {

	char* name = get_attribute(context, attributes, ATTR_NAME);
	char* value = get_attribute(context, attributes, ATTR_VALUE);

	coll_predicate_t predicate;
	predicate.predicate = &string_equals;
	predicate.data = name;
	char* prec_name = hashset_find(context->strings, &predicate);

	if(prec_name==NULL) {
		prec_name = name;
		hashset_add(context->strings, prec_name);
	}
	else {
		free(name);
	}

	// FIXME check notnull

	hashmap_put(context->capabilities, prec_name, value);
}

static void end_capability(parse_context_t* context) {

}

static void start_device(parse_context_t* context, const xmlChar** attributes) {

	context->current_device_id = get_attribute(context, attributes, ATTR_ID);
	context->current_device_user_agent = get_attribute(context, attributes, ATTR_USER_AGENT);
	context->current_device_fall_back = get_attribute(context, attributes, ATTR_FALL_BACK);

	char* att_root = get_attribute(context, attributes, ATTR_ACTUAL_DEVICE_ROOT);
	context->current_device_actual_device_root = att_root!=NULL && strcmp(att_root, "true") == 0;
	// FIXME check

	context->capabilities = hashmap_create(&string_hash, &string_rehash, &string_cmp);
}

static void end_device(parse_context_t* context) {

	devicedef_t* devicedef = devicedef_create(context->current_device_id,
			context->current_device_user_agent,
			context->current_device_fall_back,
			context->current_device_actual_device_root,
			context->capabilities);

	hashset_add(context->devices, devicedef);

	context->current_device_id = NULL;
	context->current_device_user_agent = NULL;
	context->current_device_fall_back = NULL;
	context->current_device_actual_device_root = false;
	context->capabilities = NULL;

	context->size++;
}

static void start_element(void* ctx,
		const xmlChar *name,
		const xmlChar **attributes) {

	parse_context_t* context = (parse_context_t*) ctx;

	if (xmlStrEqual(name,ELEM_DEVICE)) {
		start_device(context, attributes);
	} else if (xmlStrEqual(name, ELEM_CAPABILITY)) {
		start_capability(context, attributes);
	} else {
		// ignore other
	}
}

static void end_element(void *ctx, const xmlChar *name) {

	parse_context_t* context = (parse_context_t*) ctx;

	if (xmlStrEqual(name, ELEM_DEVICE)) {
		end_device(context);
	} else if (xmlStrEqual(name, ELEM_CAPABILITY)) {
		end_capability(context);
	} else {
		// ignore other
	}
}

static void attributef(void *ctx,
				const xmlChar *elem,
				const xmlChar *fullname,
				int type,
				int def,
				const xmlChar *defaultValue,
				xmlEnumerationPtr tree) {
	// Empty
}

static void error(void *ctx, char* msg, ...) {
	printf("%s", msg);
}

static void warn(void *ctx, char* msg, ...) {
	printf("%s",msg);
}

static void fatal(void *ctx, char* msg, ...) {
	printf("%s",msg);
}

static void nop(){
	// Empty
}

resource_data_t* resource_parse(resource_t* resource) {

	parse_context_t context;
	memset(&context, 0, sizeof(context));

	context.strings = hashset_create(&string_hash, &string_rehash, &string_cmp);

	xmlSAXHandler saxHandler;
	memset(&saxHandler, '\0', sizeof(saxHandler));

	xmlSAXVersion(&saxHandler, 1);
	saxHandler.startDocument = &start_document;
	saxHandler.endDocument = &nop;
	saxHandler.startElement = &start_element;
	saxHandler.endElement = &end_element;

	saxHandler.characters = &nop;
	saxHandler.cdataBlock = &nop;
	saxHandler.comment = &nop;
	saxHandler.error = &error;
	saxHandler.fatalError = &fatal;
	saxHandler.warning = &warn;

	int sax_error = xmlSAXUserParseFile(&saxHandler, &context, resource->path);

	hashset_destroy(context.strings);

	resource_data_t* resource_data = malloc(sizeof(resource_data_t));
	resource_data->devices = context.devices;

	xmlCleanupParser();

	return resource_data;
}
