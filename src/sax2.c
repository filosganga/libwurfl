/* Copyright (C) 2011 Fantayeneh Asres Gizaw, Filippo De Luca
 *  
 * This file is part of libWURFL.
 *
 * libWURFL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or 
 * any later version.
 *
 * libWURFL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libWURFL.  If not, see <http://www.gnu.org/licenses/>.
 */
 
/* Written by Filippo De Luca <me@filippodeluca.com>.  */

#include "parser.h"

#include "devicedef.h"
#include "utils/utils.h"
#include "utils/hashmap.h"
#include "utils/hashtable.h"
#include "utils/error.h"

#include <libxml/encoding.h>
#include <libxml/xmlversion.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <libxml/SAX2.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>
#include <iconv.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>

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

extern int errno;

typedef struct {
	hashmap_t* devices;
	hashtable_t* capabilities;
	char* version;

	devicedef_t* current_devicedef;
} parse_context_t;

static void decode_string(char* dst, const xmlChar* src) {

	char* output = dst;
	char* input = (char*)src;
	size_t inputlen = strlen(input);

	size_t inputleft = inputlen;
	size_t outputleft = xmlStrlen(src);

	iconv_t* cd = iconv_open("ASCII", "UTF-8");
	if(cd < 0) { // returns -1 on error
		// It is very improbable
		error(2, errno, "iconv does not support UTF-8 or ASCII");
	}
	size_t converted = iconv(cd, &input, &inputleft, &output, &outputleft);
	if(converted < 0) {
		error(2, errno, "error converting string");
	}

	iconv_close(cd);

}

static char* create_string(const xmlChar* string, parse_context_t* context) {

	char tmp[8 * 1024];
	memset(tmp, '\0', 8 * 1024);

	decode_string(tmp, string);

	// TODO intern the string?
	char* output = malloc(sizeof(char) * (strlen(tmp) + 1));
	if(!output) {
		error(1, errno, "error allocating string");
	}
	strcpy(output, tmp);

	return output;
}

static char* create_capability_name(const xmlChar* string, parse_context_t* context) {

	char tmp[8 * 1024];
	memset(tmp, '\0', 8 * 1024);

	decode_string(tmp, string);

	char* name = hashtable_get(context->capabilities, tmp);
	if(!name) {
		name = malloc(sizeof(char) * (strlen(tmp) + 1));
		if(!name) {
			error(1, errno, "error allocating capability name");
		}

		strcpy(name, tmp);
		hashtable_add(context->capabilities, name, NULL, NULL);
	}

	return name;

}


static xmlChar* get_attribute(parse_context_t* context, int nb_attributes,	const xmlChar** attributes, const xmlChar* name) {

	xmlChar *value=NULL;

	int index;
	for(index=0; index<nb_attributes && value==NULL; index++) {
		int att_index = index * 5;

		const xmlChar *localname = attributes[att_index];
		if(xmlStrEqual(name, localname)) {

			//const xmlChar *prefix = attributes[att_index+1];
			//const xmlChar *nsURI = attributes[att_index+2];
			const xmlChar *valueBegin = attributes[att_index+3];
			const xmlChar *valueEnd = attributes[att_index+4];

			int value_size = valueEnd - valueBegin;
			value = xmlStrsub(valueBegin, 0, value_size);
		}

	}

	return value;
}

static devicedef_t* create_devicedef(parse_context_t* context, int nb_attributes, const xmlChar** attributes) {

	const xmlChar* id = get_attribute(context, nb_attributes, attributes, ATTR_ID);
	const xmlChar* user_agent = get_attribute(context, nb_attributes, attributes, ATTR_USER_AGENT);
	const xmlChar* fallback = get_attribute(context, nb_attributes, attributes, ATTR_FALL_BACK);
	const xmlChar* actual_device_root = get_attribute(context, nb_attributes, attributes, ATTR_ACTUAL_DEVICE_ROOT);

	devicedef_t* devicedef = malloc(sizeof(devicedef_t));
	if(devicedef==NULL) {
		error(1, errno, "error allocating device");
	}
	hashmap_options_t caps_opts = {400, .75f};
	devicedef->capabilities = hashmap_init(&string_eq, &string_hash, &caps_opts);


	if(!id || xmlStrlen(id)==0) {
		error(2,0,"The device id must be != null");
	}
	else {
		devicedef->id = create_string(id, context);
	}

	if(!user_agent || xmlStrlen(user_agent)==0) {
		devicedef->user_agent = NULL;
	}
	else {
		devicedef->user_agent = create_string(user_agent, context);
	}

	if(xmlStrEqual(fallback, BAD_CAST("root")) || xmlStrlen(fallback)<=0) {
		devicedef->fall_back = NULL;
	}
	else {
		devicedef->fall_back = create_string(fallback, context);
	}
	devicedef->actual_device_root = xmlStrEqual(actual_device_root, BAD_CAST("true"));

	return devicedef;


}

// Handlers ***************************************************************

static void start_capability(parse_context_t* context, int nb_attributes, const xmlChar** attributes) {

	const xmlChar* xml_name = get_attribute(context, nb_attributes, attributes, ATTR_NAME);

	if(xml_name) {
		const xmlChar* xml_value = get_attribute(context, nb_attributes, attributes, ATTR_VALUE);

		const char* name = create_capability_name(xml_name, context);
		const char* value = create_string(xml_value, context);

		hashmap_put(context->current_devicedef->capabilities, name, value);
	}
	else {
		error(0,0,"ignore null capability name in device %s", context->current_devicedef->id);
	}
}

static void end_capability(parse_context_t* context) {

}

static void start_device(parse_context_t* context, int nb_attributes, const xmlChar** attributes) {

	context->current_devicedef = create_devicedef(context, nb_attributes, attributes);
}

static void end_device(parse_context_t* context) {

	hashmap_put(context->devices, context->current_devicedef->id, context->current_devicedef);

	// reset context
	context->current_devicedef = NULL;

}

// Sax implementation *****************************************************

static void start_document(void *ctx) {

}

static void end_document(void *ctx){

}

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
	} else {
		// ignore other
	}

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

static void sax_nop(void *ctx) {
	// Empty
}

/**
 * Parse function
 */
int parse_resource(const char* path, parser_data_t* resource_data) {

	xmlSAXHandler saxHandler;
	memset(&saxHandler, 0, sizeof(saxHandler));

	if(xmlSAXVersion(&saxHandler, 2)!=0){
		error(2,errno,"error initializing SAX2 parser");
	}

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
	context.capabilities = resource_data->capabilities;
	context.current_devicedef = NULL;

	int sax_error = xmlSAXUserParseFile(&saxHandler, &context, path);
	if(sax_error) {
		error(2, 0, "SAX error parsing file: %s", path);
	}

	error(0,0, "parsed %d devices", hashmap_size(context.devices));


	xmlCleanupParser();

	return 0;
}
