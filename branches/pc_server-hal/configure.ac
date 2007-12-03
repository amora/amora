dnl Process this file with autoconf to produce a configure script.

# (C) 2005-2006 Mandriva Conectiva S.A.
# (C) 2006 Ademar de Souza Reis Jr. <ademar@ademar.org>
#
# Based on sniffdet configure.ac: http://sniffdet.sourceforge.net
# Licensed under GNU-GPL

AC_INIT(Amora Server, 0.9svn, savagobr@yahoo.com, amora_server)
AC_PREREQ(2.58)

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_AUX_DIR([scripts])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.7.9])

AC_CONFIG_HEADERS([config.h])

AC_GNU_SOURCE
AC_CANONICAL_HOST
AM_MAINTAINER_MODE

dnl Checks for programs.
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_CC

dnl Checks for header files.
AC_CHECK_HEADERS([stdlib.h stdio.h string.h sys/socket.h sys/timeb.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_TIME
AC_TYPE_SIGNAL
AC_STRUCT_TM

AC_FUNC_STRFTIME
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([strchr printf])
AC_CHECK_FUNCS([select socket bind accept])

dnl Checks for libraries.

# if the library supports pkg-config, it's nice and easy
PKG_CHECK_MODULES(LIBBLUEZ, bluez,, \
	  AC_MSG_ERROR("*** BlueZ development files not found! You need it to build $PACKAGE_NAME. ***"))
AC_SUBST(LIBBLUEZ_CFLAGS)
AC_SUBST(LIBBLUEZ_LIBS)

PKG_CHECK_MODULES(LIBX11, x11,, \
	  AC_MSG_ERROR("*** X11 development files not found! You need it to build $PACKAGE_NAME. ***"))
AC_SUBST(LIBX11_CFLAGS)
AC_SUBST(LIBX11_LIBS)

PKG_CHECK_MODULES(LIBXTST, xtst,, \
	  AC_MSG_ERROR("*** Xtst developement files not found! You need it to build $PACKAGE_NAME. ***"))
AC_SUBST(LIBXTST_CFLAGS)
AC_SUBST(LIBXTST_LIBS)


PKG_CHECK_MODULES(LIBIMLIB2, imlib2,, \
	  AC_MSG_ERROR("*** Imlib2 developement files not found! You need it to build $PACKAGE_NAME. ***"))
AC_SUBST(LIBIMLIB2_CFLAGS)
AC_SUBST(LIBIMLIB2_LIBS)

# enable configure options and automake conditionals useful for developers
# look at m4/auxdevel.m4 for detailed documentation
AC_SUBST(csourcedir, $srcdir/src)
AC_SUBST(headerdir, "")
AC_DEVEL_MACROS
AC_DEVEL_ABSDIRS
AC_DEVEL_DEFINE_INSTALL_DIRS

AC_CONFIG_FILES([Makefile
                 Doxyfile])

AC_OUTPUT

echo "
$PACKAGE_NAME $PACKAGE_VERSION configuration:
-----------------------------
  Source code location:       ${srcdir}
  Host System Type:           ${host}
  Compiler:                   ${CC}
  CFLAGS:                     ${CFLAGS} ${LIBBLUEZ_CFLAGS} ${LIBX11_CFLAGS} ${LIBXTST_CFLAGS} ${LIBIMLIB2_CFLAGS} ${ac_devel_default_warnings}
  Libraries:                  ${LIBBLUEZ_LIBS} ${LIBX11_LIBS} ${LIBXTST_LIBS} ${LIBIMLIB2_LIBS}
  Install path (prefix):      ${prefix}

  Now type 'make' to build $PACKAGE_NAME $PACKAGE_VERSION.
"
