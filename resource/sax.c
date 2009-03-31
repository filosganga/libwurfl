/*
 * handler.c
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */
#include "parser.h"
#include "../repository.h"
#include "../collection/collections.h"
#include "../collection/hashmap.h"
#include "../collection/hashset.h"

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

	hashset_t* devices;

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


static void start_document(void *ctx){

	parse_context* context = (parse_context*) ctx;

	printf("Start document\n");
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

	char* att_root = get_attribute(attributes, ATTR_ACTUAL_DEVICE_ROOT);
	if(att_root == NULL){
		att_root = "false";
	}
	context->current_device_actual_device_root = strcmp(att_root,"true") == 0;

	// FIXME check

	context->capabilities = hashmap_create(&string_hash, &string_cmp);
	context->groups = hashmap_create(&string_hash, &string_cmp);
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
		const xmlChar *name,
		const xmlChar **attributes) {

	parse_context* context = (parse_context*) ctx;

	//printf("Start element: %s\n", name);

	if (xmlStrEqual(name,(xmlChar*)"device")) {
		start_device(context, attributes);
	} else if (xmlStrEqual(name, (xmlChar*)"group")) {
		//start_group(context, attributes);
	} else if (xmlStrEqual(name, (xmlChar*)"capability")) {
		//start_capability(context, attributes);
	} else {
		// ignore other
	}
}

static void end_element(void *ctx, const xmlChar *name) {

	parse_context* context = (parse_context*) ctx;

	//printf("End element: %s\n",name);

	if (xmlStrEqual(name, (xmlChar*)"device")) {
		//end_device(context);
	} else if (xmlStrEqual(name, (xmlChar*)"group")) {
		//end_group(context);
	} else if (xmlStrEqual(name, (xmlChar*)"capability")) {
		//end_capability(context);
	} else {
		// ignore other
	}
}

static void ignorable_whitespace(void *ctx,	const xmlChar *ch, int len) {
	// Empty
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

static void error(void *ctx, char* msg) {
	printf("%s",msg);
}

static void warn(void *ctx, char* msg) {
	printf("%s",msg);
}

static void fatal(void *ctx, char* msg) {
	printf("%s",msg);
}

static void nop(){

	printf("NOP\n");
}

hashset_t* parse_resource(char* resource) {

	parse_context context;
	//memset(&context, 0, sizeof(context));

	context.devices = hashset_create(&devicedef_hash, &devicedef_equals);

	xmlSAXHandler saxHandler;
	memset(&saxHandler, '\0', sizeof(saxHandler));

	xmlSAXVersion(&saxHandler,1);
	saxHandler.startDocument = &start_document;
	saxHandler.startElement = &start_element;
//	saxHandler.startElement = &nop;
	saxHandler.endElement = &end_element;
	saxHandler.characters = &nop;
//	saxHandler.ignorableWhitespace = &nop;
//	saxHandler.attributeDecl = &nop;
//	saxHandler.cdataBlock = &nop;
//	saxHandler.comment = &nop;
//	saxHandler.elementDecl = &nop;
//	saxHandler.entityDecl = &nop;
//	saxHandler.externalSubset = &nop;
//	saxHandler.getEntity = &nop;




	int errore = xmlSAXUserParseFile(&saxHandler, &context, resource);

	printf("error: %d", errore);

	return context.devices;
}
