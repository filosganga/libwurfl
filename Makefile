SHELL = /bin/sh

srcdir = ./src

vpath %.c $(srcdir)
vpath %.h $(srcdir)
vpath %.o $(srcdir)

CDEBUG = -g
DEFS = -D_GNU_SOURCE
INCLUDES = -I. -I$(srcdir) -I/usr/include/libxml2
LIBS = -lm -lcunit -lxml2 -ldatrie -lz

CFLAGS = -O0 -std=c99 $(CDEBUG) $(INCLUDES) $(DEFS)
LDFLAGS = -g

ifdef __WIN32
	CC := i586-mingw32msvc-gcc
	CXX := i586-mingw32msvc-g++
	LD := i586-mingw32msvc-ld
	
	DEFS = -D__WIN32
	
else
	CC := gcc
endif


	
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LIBS) $(TARGET_ARCH) -c

#All Target
all: cwurfl test

# Include modules
include $(srcdir)/utils/subdir.mk
include $(srcdir)/utils/avl/subdir.mk
include $(srcdir)/utils/memory/subdir.mk
include $(srcdir)/utils/hashlib/subdir.mk
include $(srcdir)/utils/collection/subdir.mk
include $(srcdir)/utils/process/subdir.mk
include $(srcdir)/utils/thread/subdir.mk
include $(srcdir)/resource/subdir.mk
include $(srcdir)/repository/subdir.mk
include $(srcdir)/matcher/subdir.mk

OBJS := $(patsubst %.c,%.o,$(filter %.c,$(SRCS)))
TOBJS := $(patsubst %.c,%.o,$(filter %.c,$(TSRCS)))
	
cwurfl: wurfl.c $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -o $@ $?

clean:
	-rm -fR wurfl test $(OBJS) $(TOBJS) doc; \

	
test-compile: $(OBJS) $(TOBJS) unit-test.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -o test $?
	
test: test-compile
	./test
    
doc:
	doxygen Doxyfile

	
PHONY: all clean test doc