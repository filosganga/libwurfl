/*
 * xml_reader.c
 *
 *  Created on: 20/apr/2011
 *      Author: filippodeluca
 */

#include "devicedef.h"
#include "utils/error.h"
#include "utils/hashmap.h"

#include <libxml/xmlreader.h>

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

static device_t* create_device(xmlNodePtr node) {

	xmlChar* id = xmlGetProp(node, (const xmlChar*)"id");
	xmlChar* user_agent = xmlGetProp(node, (const xmlChar*)"user_agent");
	xmlChar* fall_back = xmlGetProp(node, (const xmlChar*)"fall_back");




}

void process_node(xmlTextReaderPtr reader, hashmap_t* devices) {
	int nodeType = xmlTextReaderNodeType(reader);
	xmlChar* name = xmlTextReaderLocalName(reader);

	if(nodeType == 1 && xmlStrEqual(name, "device")) {
		xmlNodePtr deviceNode = xmlTextReaderExpand(reader);
		devicedef_t* device = create_device(reader);
		hashmap_put(devices, device->id, device);
	}

}

int stream_file(const char *path) {
    xmlTextReaderPtr reader;
    int ret;

    reader = xmlNewTextReaderFilename(path);
    if (reader != NULL) {
        while((ret = xmlTextReaderRead(reader)) == 1) {
        	process_node(reader);
        }
        xmlFreeTextReader(reader);

        if (ret != 0) {
            printf("%s : failed to parse\n", filename);
        }
    } else {
    	error(2,0,"error opening file: %", path);
    }
}

