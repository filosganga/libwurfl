/*
 * handler.c
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */
#include "resource.h"

#include "utils/collection/collections.h"
#include "utils/collection/hashmap.h"
#include "utils/collection/hashtable.h"
#include "utils/collection/utils.h"
#include "utils/logger.h"

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

#define ELEM_WURFL BAD_CAST("wurfl")
#define ELEM_WURFL_PATCH BAD_CAST("wurfl_patch")
#define ELEM_CAPABILITY BAD_CAST("capability")
#define ELEM_GROUP BAD_CAST("group")
#define ELEM_DEVICE BAD_CAST("device")
#define ELEM_VERSION BAD_CAST("version")
#define ELEM_VER BAD_CAST("ver")

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

	hashtable_t* strings;
	hashtable_t* devices;
	hashmap_t* capabilities;

	logger_t* logger;

	resource_type_e type;
} parse_context_t;


static char* get_attribute(parse_context_t* context, int nb_attributes,	const xmlChar** attributes, xmlChar* name) {

	const xmlChar* value = NULL;

	int index=0;
	while(index<(nb_attributes*5) && !xmlStrEqual(name, attributes[index])) index+=5;

	if(index<(nb_attributes*5)) {
        const xmlChar *localname = attributes[index];
        const xmlChar *prefix = attributes[index+1];
        const xmlChar *nsURI = attributes[index+2];
        const xmlChar *valueBegin = attributes[index+3];
        const xmlChar *valueEnd = attributes[index+4];

        int value_size = valueEnd - valueBegin;

        if(value_size>0) {
        	value = create_string(context->strings, xmlStrsub(valueBegin, 0, value_size));
        }
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
}

static void end_document(void *ctx){

	parse_context_t* context = (parse_context_t*) ctx;
}

static void start_capability(parse_context_t* context, int nb_attributes, const xmlChar** attributes) {

	const char* name = get_attribute(context, nb_attributes, attributes, ATTR_NAME);
	const char* value = get_attribute(context, nb_attributes, attributes, ATTR_VALUE);

	// TODO check not null

	logger_debug(context->logger, __FILE__, __LINE__, "Adding capability: %s:%s", name, value);
	hashmap_put(context->capabilities, name, value);
}

static void end_capability(parse_context_t* context) {

}

static void start_device(parse_context_t* context, int nb_attributes, const xmlChar** attributes) {

	context->current_device_id = get_attribute(context, nb_attributes, attributes, ATTR_ID);
	context->current_device_user_agent = get_attribute(context, nb_attributes, attributes, ATTR_USER_AGENT);
	context->current_device_fall_back = get_attribute(context, nb_attributes, attributes, ATTR_FALL_BACK);

	char* att_root = get_attribute(context, nb_attributes, attributes, ATTR_ACTUAL_DEVICE_ROOT);
	context->current_device_actual_device_root = att_root!=NULL && strcmp(att_root, "true") == 0;

	hashmap_options_t caps_opts;
	caps_opts.initial_capacity = 400;
	caps_opts.load_factor = 0.75f;
	context->capabilities = hashmap_create(&string_eq, &string_hash, &caps_opts);
}

static void end_device(parse_context_t* context) {

	devicedef_t* devicedef = malloc(sizeof(devicedef_t));
	if(devicedef==NULL) {
		exit(-1);
	}

	context->size++;
	devicedef->id = context->current_device_id;
	devicedef->fall_back = context->current_device_fall_back;
	devicedef->actual_device_root = context->current_device_actual_device_root;
	devicedef->user_agent = context->current_device_user_agent;
	devicedef->capabilities = context->capabilities;

	logger_debug(context->logger, __FILE__, __LINE__, "Adding %dth device: %s", context->size, devicedef->id);
	hashtable_add(context->devices, devicedef);
	assert(context->size == hashtable_size(context->devices));

	// reset context
	context->current_device_id = NULL;
	context->current_device_user_agent = NULL;
	context->current_device_fall_back = NULL;
	context->current_device_actual_device_root = false;
	context->capabilities = NULL;
}

static void start_wurfl(parse_context_t* context, int nb_attributes, const xmlChar** attributes) {
	context->type = ROOT;
}

static void end_wurfl(parse_context_t* context) {

}

static void start_wurfl_patch(parse_context_t* context, int nb_attributes, const xmlChar** attributes) {
	context->type = PATCH;
}

static void end_wurfl_patch(parse_context_t* context) {

}

static void start_ver(parse_context_t* context, int nb_attributes, const xmlChar** attributes) {
	// TODO set ver started
}

static void end_ver(parse_context_t* context) {
	// TODO put ver not started
}

// Sax implementation *****************************************************

static void startElementNs (void *ctx,
					const xmlChar *localname,
					const xmlChar *prefix,
					const xmlChar *URI,
					int nb_namespaces,
					const xmlChar **namespaces,
					int nb_attributes,
					int nb_defaulted,
					const xmlChar **attributes) {

	parse_context_t* context = (parse_context_t*) ctx;

	if (xmlStrEqual(localname,ELEM_DEVICE)) {
		start_device(context, nb_attributes, attributes);
	} else if (xmlStrEqual(localname, ELEM_CAPABILITY)) {
		start_capability(context, nb_attributes, attributes);
	} else if (xmlStrEqual(localname, ELEM_VER)) {
		start_ver(context, nb_attributes, attributes);
	} else if (xmlStrEqual(localname, ELEM_WURFL)) {
		start_wurfl(context, nb_attributes, attributes);
	} else if (xmlStrEqual(localname, ELEM_WURFL_PATCH)) {
		start_wurfl_patch(context, nb_attributes, attributes);
	} else {
		// ignore other
	}
}

void endElementNs(void *ctx,
					const xmlChar *localname,
					const xmlChar *prefix,
					const xmlChar *URI) {

	parse_context_t* context = (parse_context_t*) ctx;

	if (xmlStrEqual(localname, ELEM_DEVICE)) {
		end_device(context);
	} else if (xmlStrEqual(localname, ELEM_CAPABILITY)) {
		end_capability(context);
	} else if (xmlStrEqual(localname, ELEM_VER)) {
		end_ver(context);
	} else if (xmlStrEqual(localname, ELEM_WURFL)) {
		end_wurfl(context);
	} else if (xmlStrEqual(localname, ELEM_WURFL_PATCH)) {
		end_wurfl_patch(context);
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

static void chars(void *ctx, const xmlChar *ch, int len) {
	// TODO add version reader
}

static void sax_error(void *ctx, const char *msg, ...) {
	printf("%s", msg);
}

static void sax_warn(void *ctx, const char* msg, ...) {
	printf("%s",msg);
}

static void sax_fatal(void *ctx, const char* msg, ...) {
	printf("%s",msg);
}

static void sax_nop(){
	// Empty
}

int resource_parse(resource_data_t* resource_data, const char* path, hashtable_t* strings) {

	xmlSAXHandler saxHandler;
	memset(&saxHandler, 0, sizeof(saxHandler));

	xmlSAXVersion(&saxHandler, 2);
	saxHandler.startDocument = &start_document;
	saxHandler.endDocument = &end_document;

	saxHandler.startElementNs = &startElementNs;
	saxHandler.endElementNs = &endElementNs;

	saxHandler.characters = &chars;
	saxHandler.cdataBlock = &sax_nop;
	saxHandler.comment = &sax_nop;
	saxHandler.error = &sax_error;
	saxHandler.fatalError = &sax_fatal;
	saxHandler.warning = &sax_warn;

	// Creating context
	parse_context_t context;
	memset(&context, 0, sizeof(context));

	context.devices = resource_data->devices;
	context.strings = strings;
	context.logger = logger_init();

	fprintf(stderr, "Starting parsing\n");
	int sax_error = xmlSAXUserParseFile(&saxHandler, &context, path);
	fprintf(stderr, "Sax error: %d\n", sax_error);

	resource_data->version = strdup("TBD");
	resource_data->type = context.type;

	logger_destroy(context.logger);
	xmlCleanupParser();

	return 0;
}
