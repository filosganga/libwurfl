/*
 * handler.c
 *
 *  Created on: 23-mar-2009
 *      Author: filosganga
 */
#include "parser.h"
#include "../repository.h"
#include "../collection/list.h"

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

struct _parse_context {
	device_definition* curent_device;
	char* current_group_id;
	List* devices;
};

typedef struct _parse_context parse_context;


static int device_definition_equals(const void *litem, const void *ritem) {

	device_definition* ldev = (device_definition*)litem;
	device_definition* rdev = (device_definition*)ritem;

	return ldev->id == rdev->id;
}

static void* device_definition_clone(const void *item) {

	return item;
}

static void device_definition_free (void *item) {

}

static void start_device(parse_context* context, const xmlChar** attributes) {

}

static void start_group(parse_context* context, const xmlChar** attributes) {

}

static void start_capability(parse_context* context, const xmlChar** attributes) {

}

static void end_device(parse_context* context) {

}

static void end_group(parse_context* context) {

}

static void end_capability(parse_context* context) {

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

/*
 *
 */
device_definition** parse_resource(char* resource) {

	parse_context context;
	context.devices = list_create(&device_definition_equals, &device_definition_clone, &device_definition_free);

	xmlSAXHandler saxHandler; // See http://xmlsoft.org/html/libxml-tree.html#xmlSAXHandler
	memset(&saxHandler, 0, sizeof(saxHandler));

	xmlSAXVersion(&saxHandler, 2);
	saxHandler.startElementNs = start_element;
	saxHandler.endElementNs = end_element;

	xmlSAXUserParseFile(&saxHandler, &context, resource);

	device_definition** devices = NULL;

	return devices;
}
