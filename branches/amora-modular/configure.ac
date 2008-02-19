dnl Process this file with autoconf to produce a configure script.

# (C) 2005-2006 Mandriva Conectiva S.A.
# (C) 2006 Ademar de Souza Reis Jr. <ademar@ademar.org>
#
# Based on sniffdet configure.ac: http://sniffdet.sourceforge.net
# Licensed under GNU-GPL

AC_INIT(Amora Server, 1.0svn, savagobr@yahoo.com, amora-server)
AC_PREREQ(2.58)

AC_CONFIG_SRCDIR([src/amora.c])
AC_CONFIG_AUX_DIR([scripts])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.7.9])

AC_CONFIG_HEADERS([config.h])

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

dnl Check if pkg-config is installed
AC_CHECK_PROG(PKGCONFIG, pkg-config, yes, no)
if test $PKGCONFIG = "no"; then
	AC_MSG_ERROR("*** pkg-config not found! ***")
fi

dnl Checks for libraries.

dnl If the library supports pkg-config, it's nice and easy
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

dnl Optional D-Bus support for bluetooth dongle detection
AC_ARG_ENABLE(dbus, AS_HELP_STRING([--disable-dbus],[Disable D-BUS support]),,[enable_dbus=yes])
if test "x$enable_dbus" = "xyes"; then
	PKG_CHECK_MODULES(LIBDBUS, dbus-1, [have_dbus=yes], [have_dbus=no])
	AC_SUBST(LIBDBUS_CFLAGS)
	AC_SUBST(LIBDBUS_LIBS)
else
	have_dbus="no (disabled)"
fi

if test "x$have_dbus" = "xyes"; then
	AC_DEFINE(HAVE_DBUS, [], [Define if D-Bus is present])

	dnl dbus_watch_get_unix_fd was introduced in D-Bus 1.1.1 as replacement for dbus_watch_get_fd
	AC_CHECK_LIB(dbus-1, dbus_watch_get_unix_fd, [], 
		     AC_DEFINE(DBUS_COMPAT_MODE, [], [Older libdbus-1 version]))
fi

AM_CONDITIONAL(HAVE_DBUS, test "x$have_dbus" = "xyes")

dnl Enable configure options and automake conditionals useful for developers
dnl Look at m4/auxdevel.m4 for detailed documentation
AC_SUBST(csourcedir, $srcdir/src)
AC_SUBST(utestdir, $srcdir/utests)
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
  D-Bus support:              ${have_dbus}
  CFLAGS:                     ${CFLAGS} ${LIBBLUEZ_CFLAGS} ${LIBX11_CFLAGS} ${LIBXTST_CFLAGS} ${LIBIMLIB2_CFLAGS} ${LIBDBUS_CFLAGS} ${ac_devel_default_warnings}
  Libraries:                  ${LIBBLUEZ_LIBS} ${LIBX11_LIBS} ${LIBXTST_LIBS} ${LIBIMLIB2_LIBS} ${LIBDBUS_LIBS}
  Install path (prefix):      ${prefix}

  Now type 'make' to build $PACKAGE_NAME $PACKAGE_VERSION.
"
