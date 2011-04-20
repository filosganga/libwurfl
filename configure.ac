#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([WURFL C API], [1.0], [me@filippodeluca.com],[].[])
AC_CONFIG_AUX_DIR([build])
AC_CONFIG_SRCDIR([src/wurfl.h])
AC_CONFIG_HEADER([config.h:config.h.in])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign])
LT_INIT

AC_INCLUDES_DEFAULT([/usr/include /usr/local/include])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# Checks for libraries.
AC_SEARCH_LIBS([xmlCleanupParser],[xml xml2])
AC_SEARCH_LIBS([xmlSAXVersion],[xml xml2])
AC_SEARCH_LIBS([xmlSAXUserParseFile],[xml xml2])
AC_SEARCH_LIBS([min],[m])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h memory.h stddef.h stdint.h stdlib.h string.h strings.h math.h libxml/SAX2.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
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
AC_CHECK_FUNCS([memset strdup])

AC_CONFIG_FILES([Makefile src/Makefile]) 
AC_OUTPUT