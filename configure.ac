#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([libwurfl], [0.1], [me@filippodeluca.com])
AC_CONFIG_AUX_DIR([build])
AC_CONFIG_SRCDIR([src/wurfl.h])
AC_CONFIG_HEADER([src/config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign])
LT_INIT

LDFLAGS+=" -L/usr/lib"
CFLAGS+=" -I/usr/include"

# For custom installs.
LDFLAGS+=" -L/usr/local/lib"
CFLAGS+=" -I/usr/local/include"

# For MacPorts.
LDFLAGS+=" -L/opt/local/lib"
CFLAGS+=" -L/opt/local/include"

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# Checks for libraries.
AC_SEARCH_LIBS([xmlSAXVersion],[xml2], [
	CFLAGS+=" -I/usr/include/libxml2"
], [
	AC_MSG_ERROR([Failed to locate the xml2])
])
AC_SEARCH_LIBS([iconv_open],[iconv], [], [
	AC_MSG_ERROR([Failed to locate the iconv library])
])



# Check for Apache
AC_ARG_WITH(apache, [  --with-apache        Specify installation directory for Apache 2.2.x], [
    LDFLAGS+=" -L$with_apache/lib"
    CFLAGS+=" -I$with_apache/include"
    AC_PATH_PROG([APXS], apxs, , $with_apache/bin)
    AC_CHECK_HEADER(apr.h,, [AC_MSG_ERROR(Apache Portable Runtime headers not found.)])
    AC_CHECK_HEADER(httpd.h,, [AC_MSG_ERROR(Apache headers not found.)])
],[
    # Look for Apache Portable Runtime
    if test -d /usr/include/apr-1
    then
        CFLAGS+=" -I/usr/include/apr-1"
    elif test -d /usr/include/apr-1.0
    then
        CFLAGS+=" -I/usr/include/apr-1.0"
    fi

    CFLAGS+=" -I/usr/include/apache2"

    AC_CHECK_HEADER(apr.h,, [AC_MSG_ERROR(Apache Portable Runtime headers not found.)])
    AC_CHECK_HEADER(httpd.h,, [AC_MSG_ERROR(Apache headers not found.)])
    AC_PATH_PROG([APXS], apxs)
])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h memory.h stddef.h stdint.h stdlib.h string.h strings.h math.h libxml/SAX2.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_HEADER_ASSERT
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_INT32_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([memset strdup strstr strcpy strcmp getline getdelim])

AC_CONFIG_FILES([Makefile src/Makefile]) 
AC_OUTPUT
