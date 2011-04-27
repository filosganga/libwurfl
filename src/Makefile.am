INCLUDES = -I$(top_srcdir)

nobase_include_HEADERS =  \
	wurfl.h \
	device.h 
	
lib_LTLIBRARIES = libwurfl.la
libwurfl_la_SOURCES = \
	wurfl.c \
	device.c \
	devicedef.c \
	repository.c \
	matcher.c \
	normalizer.c \
	sax2.c \
	utils/functors.c \
	utils/hashmap.c \
	utils/hashtable.c \
	utils/linkedlist.c \
	utils/patricia.c \
	utils/error.c \
	utils/utils.c 
	
libwurfl_la_LDFLAGS = -version-info 1:0:0
libwurfl_la_CPPFLAGS = -I/usr/include -I/usr/local/include -I/usr/include/libxml2

check_PROGRAMS = test-static
#test_SOURCES = test.c
#test_LDADD = libwurfl.la
#test_CPPFLAGS = -I/usr/include -I/usr/local/include

test_static_SOURCES = test.c
test_static_LDADD = libwurfl.la
test_static_LDFLAGS = -static
test_static_CPPFLAGS = -I/usr/include -I/usr/local/include -I/usr/include/libxml2

TESTS = $(check_PROGRAMS)

