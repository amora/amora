# based on AC_DEFINE_DIR from
# http://autoconf-archive.cryp.to/ac_define_dir.html
#
# This macro _AC_DEFINEs variables to the expansion of
# installation directories variables, taking care of
# fixing up ${prefix} and such.
#
# The exported variables are offered as both a C preprocessor
# symbol (through config.h or similar) and an output variable
# (which can be used as output from configure). They're named
# after the original name, but using upper-case.
#
# a double eval is used because starting on autoconf 2.60 some
# variables reference other variables, so it's better to be on
# the safe side.
#

AC_DEFUN([AC_DEVEL_DEFINE_INSTALL_DIRS], [
  prefix_NONE=
  exec_prefix_NONE=
  test "x$prefix" = xNONE && prefix_NONE=yes && prefix=$ac_default_prefix
  test "x$exec_prefix" = xNONE && exec_prefix_NONE=yes && exec_prefix=$prefix

  eval ac_define_dir="$bindir"
  eval ac_define_dir="\"$ac_define_dir\""
  AC_SUBST(BINDIR, "$ac_define_dir")
  AC_DEFINE_UNQUOTED(BINDIR, "$ac_define_dir",
      [dir for installing executables that users
      run. Example: /usr/local/bin])
  eval ac_define_dir="$datadir"
  eval ac_define_dir="\"$ac_define_dir\""
  AC_SUBST(DATADIR, "$ac_define_dir")
  AC_DEFINE_UNQUOTED(DATADIR, "$ac_define_dir",
      [dir for installing modifiable single-machine
      data. Example: /usr/local/share])
  eval ac_define_dir="$prefix"
  eval ac_define_dir="\"$ac_define_dir\""
  AC_SUBST(PREFIX, "$ac_define_dir")
  AC_DEFINE_UNQUOTED(PREFIX, "$ac_define_dir",
      [installation prefix for all files.
      Example: /usr/local])
  eval ac_define_dir="$exec_prefix"
  eval ac_define_dir="\"$ac_define_dir\""
  AC_SUBST(EXEC_PREFIX, "$ac_define_dir")
  AC_DEFINE_UNQUOTED(EXEC_PREFIX, "$ac_define_dir",
      [installation prefix for architecture-dependent
      files. Example: /usr/local])
  eval ac_define_dir="$includedir"
  eval ac_define_dir="\"$ac_define_dir\""
  AC_SUBST(INCLUDEDIR, "$ac_define_dir")
  AC_DEFINE_UNQUOTED(INCLUDEDIR, "$ac_define_dir",
      [dir for installing C header files.
      Example: /usr/local/include])
  eval ac_define_dir="$libdir"
  eval ac_define_dir="\"$ac_define_dir\""
  AC_SUBST(LIBDIR, "$ac_define_dir")
  AC_DEFINE_UNQUOTED(LIBDIR, "$ac_define_dir",
      [dir for installing object code libraries.
      Example: /usr/local/lib])
  eval ac_define_dir="$libexecdir"
  eval ac_define_dir="\"$ac_define_dir\""
  AC_SUBST(LIBEXECDIR, "$ac_define_dir")
  AC_DEFINE_UNQUOTED(LIBEXECDIR, "$ac_define_dir",
      [dir for installing executables that other programs
      run. Example: /usr/local/libexec])
  eval ac_define_dir="$localstatedir"
  eval ac_define_dir="\"$ac_define_dir\""
  AC_SUBST(LOCALSTATEDIR, "$ac_define_dir")
  AC_DEFINE_UNQUOTED(LOCALSTATEDIR, "$ac_define_dir",
      [dir for installing modifiable single-machine data.
      Examples: /usr/local/var, /var])
  eval ac_define_dir="$sbindir"
  eval ac_define_dir="\"$ac_define_dir\""
  AC_SUBST(SBINDIR, "$ac_define_dir")
  AC_DEFINE_UNQUOTED(SBINDIR, "$ac_define_dir",
      [dir for installing executables that sys-admins
      run. Example: /usr/local/sbin])
  eval ac_define_dir="$sharedstatedir"
  eval ac_define_dir="\"$ac_define_dir\""
  AC_SUBST(SHAREDSTATEDIR, "$ac_define_dir")
  AC_DEFINE_UNQUOTED(SHAREDSTATEDIR, "$ac_define_dir",
      [dir for installing modifiable architecture-independent
      data. Example: /usr/local/com])
  eval ac_define_dir="$sysconfdir"
  eval ac_define_dir="\"$ac_define_dir\""
  AC_SUBST(SYSCONFDIR, "$ac_define_dir")
  AC_DEFINE_UNQUOTED(SYSCONFDIR, "$ac_define_dir",
      [dir for installing read-only single-machine data.
      Examples: /usr/local/etc, /etc])
  eval ac_define_dir="$mandir"
  eval ac_define_dir="\"$ac_define_dir\""
  AC_SUBST(MANDIR, "$ac_define_dir")
  AC_DEFINE_UNQUOTED(MANDIR, "$ac_define_dir",
      [top-level dir for installing documentation in man format.
      Example: /usr/local/man])
  eval ac_define_dir="$infodir"
  eval ac_define_dir="\"$ac_define_dir\""
  AC_SUBST(INFODIR, "$ac_define_dir")
  AC_DEFINE_UNQUOTED(INFODIR, "$ac_define_dir",
      [top-level dir for installing documentation in info format.
      Example: /usr/local/info])

  test "$prefix_NONE" && prefix=NONE
  test "$exec_prefix_NONE" && exec_prefix=NONE
])
