libWURFL - WURFL Shared Library
==============================
It is a very simple C library used to parse the WURFL file and match the
HTTP request to obtain the mobile device capabilities.

The interface is described in the wurfl.h and device.h files.

Quick Start
-----------
To initialize the WURLF lib you must download the last WURLF xml file. 
It can be downloaded from:
    
    http://sourceforge.net/projects/wurfl/files/WURFL/latest/wurfl-latest.xml.gz/download

The wurfl operations can be done by mean the wurfl_t object, ok the C language 
does not have the Object concept, but I refer to the ADT. To initialize such 
object you need to call the wurfl_init function:

    const char* root = "/path/to/wurfl.xml";
    const char* patches[] = {"/path/to/patch1", "/path/to/patch2"};
    wurfl_t* wurfl = wurfl_init(root, patches);

Once the wurfl object is initialized, you can use it to match the HTTP request to 
obtain a device. The only request field we need is the User-Agent header value.
In 99% of the cases it is sufficient to obtain a right device detection. The 
user_agent must be encoded as ASCII string, no other encoding are supported.

    const char* user_agent = ...; // Obtained from the HTTP request
    device_t* device = wurfl_match(wurfl, user_agent);

The device_t ADT represent the matched device, it enable you to access to the 
capabilities values. Different from the official WURFL api, this device may
have different number of capability from the generic device. This choice is due
to the fact that not all the capability make sense to all device. Having a not
null capability setted to "" is equal to not having it.

    char* capability_value = device_capability(device, "capability_name");

You can also obtain all capability from the device. They are stored in an array
of char*: [name_0, value_0, name_1, value_1, ...., name_n, value_n, NULL]

    char** capabilities = device_capabilities(device, NULL);
    char** caps_ptr = capabilities;
	
    while(caps_ptr!=NULL && *caps_ptr!=NULL) {
        fprintf(stderr, "%s: %s,\n", *caps_ptr, *(caps_ptr + 1));
        caps_ptr+=2;
    }
    
Enjoy with that.
