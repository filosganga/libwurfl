# Copyright (C) 2011 Fantayeneh Asres Gizaw, Filippo De Luca
#   
# This file is part of libWURFL.
# 
# libWURFL is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or 
# any later version.
# 
# libWURFL is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with libWURFL.  If not, see <http://www.gnu.org/licenses/>.

INCLUDES = -I$(top_srcdir)

nobase_include_HEADERS =  \
	wurfl.h \
	device.h 
	
lib_LTLIBRARIES = libwurfl.la
libwurfl_la_SOURCES = \
	wurfl.c \
	device.c \
	devicedef.c \
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
	
libwurfl_la_LDFLAGS = -version-info 0:0:0

check_PROGRAMS = test

test_SOURCES = test.c
test_LDADD = libwurfl.la
test_LDFLAGS = -static
#test_CPPFLAGS = -I/usr/include -I/usr/local/include -I/usr/include/libxml2

TESTS = $(check_PROGRAMS)

