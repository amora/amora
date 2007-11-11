# this is my first m4 script, the style is probably completely wrong
# TODO: read carefully the docs about quotation, quote and indent properly

# Make a path absolute and remove "..", ".", "//", final slash.
#
# If you find a bug, please fix it also in the sanitize_path bash script
# (and test with the test-sanitize script).
# AC_SANITIZE_PATH([/path/to/be/sanitized], [output_variable_name])
#
# remember that `@<:@' = `[' and `@:>@' = `]'
# TODO: change AC_MSG_NOTICEs with a proper debug macro
AC_DEFUN([AC_SANITIZE_PATH],
[

# AC_MSG_NOTICE([Sanitize: called with <$1>, <$2>]) # <-

ac_nomiddledot='s;/\./;/;g'
ac_noenddot='s;/\./\?$;;'
ac_noslashslash='s;//\+;/;g'
ac_nodotdot='s;@<:@^/@:>@*/\.\.\(/\|$\);;' # note: this removes only one foo/..
ac_nofinalslash='s;\(.\)/$;\1;g'

# absolutize
if echo "$1" | grep '^/' >/dev/null 2>&1 ; then
#    AC_MSG_NOTICE([Sanitize: I think <$1> is an absolute directory]) # <-
    $2="$1"
else
#    AC_MSG_NOTICE([Sanitize: I think <$1> is a relative directory]) # <-
    $2="`pwd -P`/$1"
#    AC_MSG_NOTICE([Sanitize: Absolutized to <${$2}>]) # <-
fi

# remove single dots
for ac_transform in "$ac_nomiddledot" "$ac_noenddot"; do
    $2="`echo "${$2}" | sed -e "$ac_transform"`"
#    AC_MSG_NOTICE([Sanitize: Removed dots to <${$2}>]) # <-
done

# remove spurious slashes
$2=`echo "${$2}" | sed -e "$ac_noslashslash"`
# AC_MSG_NOTICE([Sanitize: Removed slashes to <${$2}>]) # <-

# one while to remove them all
while echo "${$2}" | grep '\.\.\(/\|$\)' >/dev/null 2>&1; do
    $2=`echo "${$2}" | sed -e "$ac_nodotdot"`
#    AC_MSG_NOTICE([Sanitize: Removed dotdots to <${$2}>]) # <-
done

# remove final slash
$2=`echo "${$2}" | sed -e "$ac_nofinalslash"`
# AC_MSG_NOTICE([Sanitize: Removed final slash to <${$2}>]) # <-

if ! test "${$2}" ; then
# Got an empty path, I hope it's root...
#    AC_MSG_NOTICE([Sanitize: After all that, I think it is / ]) # <-
    $2=/
fi
# AC_MSG_NOTICE([Sanitize: from $1, defined $2 to <${$2}>])
])

# create and AC_SUBS sanitized abs_ versions of the following variables:
# - csourcedir
# - headerdir
# - utestdir
# - docdir
# - mytestdir
AC_DEFUN([AC_DEVEL_ABSDIRS], [

if test "$csourcedir"; then
  AC_SANITIZE_PATH([$csourcedir], abs_csourcedir)
  AC_SUBST(abs_csourcedir)
fi
if test "$headerdir"; then
  AC_SANITIZE_PATH([$headerdir], abs_headerdir)
  AC_SUBST(abs_headerdir)
fi
if test "$utestdir"; then
  AC_SANITIZE_PATH([$utestdir], abs_utestdir)
  AC_SUBST(abs_utestdir)
fi
if test "$docdir"; then
  AC_SANITIZE_PATH([$docdir], abs_docdir)
  AC_SUBST(abs_docdir)
fi
if test "$mytestdir"; then
  AC_SANITIZE_PATH([$mytestdir], abs_mytestdir)
  AC_SUBST(abs_mytestdir)
fi
if test "$lcov_ignore_dirs"; then
  AC_SANITIZE_PATH([$lcov_ignore_dirs], abs_lcov_ignore_dirs)
  AC_SUBST(abs_lcov_ignore_dirs)
fi

])

# This macro gives configure options for enabling Check unit testing, debugging
# symbols, efence, LCOV, and a standard set of warnings.  It also provides the
# --enable-devel, which sets all five options.
#
# For each option foo, if --enable-foo was asked, AM_AUX_DEVEL sets
# shell variable $ac_foo_enabled to 'yes' and Automake conditional FOO to
# true.
#
# --enable-check calls AM_PATH_CHECK.  Note that if check.m4 was
#   not found in the *maintainer's* machine (when autoconf is run), the
#   resulting configure script will not have check support!
#
# --enable-debug is somewhat intrusive.  It defines DEBUG and
#   forces CFLAGS to "-g -O0", overriding the user environment variable.
#
# --enable-efence checks for the efence library.
#
# --enable-lcov checks for gcov and lcov.
#
# --enable-warnings adds a lot of extra gcc warnings flags in the
#   variable ac_devel_default_warnings.
#
# See also the file auxdevel.am, with Makefile usage of the conditionals set by
# these options.
#
# TODO: check for Doxygen binary

AC_DEFUN([AC_DEVEL_MACROS],
[

ac_isodate="`date +%Y-%m-%d`"
AC_SUBST(ac_isodate)

AC_ARG_ENABLE(warnings,
  AS_HELP_STRING([--enable-warnings], [Enable extra GCC warnings flags]),
    [ac_warnings_enabled=$enableval], [ac_warnings_enabled=no])
AC_ARG_ENABLE(efence,
  AS_HELP_STRING([--enable-efence], [Enable EletricFence]),
    [ac_efence_enabled=$enableval], [ac_efence_enabled=no])
AC_ARG_ENABLE(debug,
  AS_HELP_STRING([--enable-debug],
    [Define DEBUG, force CFLAGS to "-g -O0" (overrides environment variable)]),
  [ac_debug_enabled=$enableval], [ac_debug_enabled=no])
AC_ARG_ENABLE(lcov,
  AS_HELP_STRING([--enable-lcov], [Enable gcov/lcov support]),
    [ac_lcov_enabled=$enableval], [ac_lcov_enabled=no])

AC_ARG_WITH(buildversion,
  AS_HELP_STRING([--with-buildversion=BUILDVERSION],
    [Build version string (default: ISO date of configuration]),
  [ac_buildversion=$withval],
  [ac_buildversion="$ac_isodate"])


# Unfortunately this m4_ifdef doesn't work with older versions of autotools, so
# if you want to support them, you should distribute check.m4 with your package.
# Failing to do that will result in a cryptic error message if the user don't
# have Check installed.
#
# Also note that if you use this feature and don't have Check when calling
# autoconf, the resulting `configure' script will lack Check support.  This is
# probably not what you want, so make sure that the maintainer has Check
# installed when running `make dist'.
m4_ifdef([AM_PATH_CHECK],
  [
    AC_ARG_ENABLE(check,
      AS_HELP_STRING([--enable-check], [Enables unit testing with Check]),
                     [ac_check_enabled=$enableval], [ac_check_enabled=no])
    AC_ARG_ENABLE(devel,
      AS_HELP_STRING([--enable-devel],
       [Equivalent to --enable-warnings --enable-efence --enable-debug
       --enable-check --enable-lcov]),
    [ac_warnings_enabled=$enableval
     ac_efence_enabled=$enableval
     ac_debug_enabled=$enableval
     ac_check_enabled=$enableval
     ac_lcov_enabled=$enableval])
 ],
 [
    AC_DIAGNOSE([syntax], [Could not find check.m4, no support for unit testing.])
    AC_ARG_ENABLE(devel,
      AS_HELP_STRING([--enable-devel],
       [Equivalent to --enable-warnings --enable-efence --enable-debug
       --enable-lcov]),
    [ac_warnings_enabled=$enableval
     ac_efence_enabled=$enableval
     ac_debug_enabled=$enableval
     ac_lcov_enabled=$enableval])
 ])

if test "$ac_warnings_enabled" = yes; then
  AC_SUBST(ac_devel_default_warnings, ["\
-Wmissing-declarations -Wmissing-prototypes \
-Wredundant-decls -Wshadow -Wbad-function-cast \
-Wcast-qual"])
else
  AC_SUBST(ac_devel_default_warnings, ["-Wall -W"])
fi

if test "$ac_efence_enabled" = yes; then
  AC_CHECK_LIB(efence, malloc, [],
    [AC_MSG_ERROR([Linking to efence asked, but could not find the library])])
fi

m4_ifdef([AM_PATH_CHECK],
  [if test "$ac_check_enabled" = yes; then
    AM_PATH_CHECK
   fi])

# TODO: find unobtrusive way of disabling optimization and enabling debug
if test "$ac_debug_enabled" = yes; then
  CFLAGS="-g -O0"
  AC_MSG_WARN([Using --enable-debug, overriding user CFLAGS.])
  AC_DEFINE([DEBUG], [1], [Set if in debug mode])
else
  AC_DEFINE([NDEBUG], [1], [Set if not in debug mode])
fi

# TODO: support --with-lcov, --with-gcov
# TODO: check also for lcov.pl, genhtml.pl, etc.
if test "$ac_lcov_enabled" = yes; then
  AC_CHECK_PROGS(gcov, gcov)
  if ! test "$gcov"; then
    AC_MSG_ERROR([Could not find gcov utility, check your gcc instalation])
  fi
  AC_CHECK_LIB(gcov, __gcov_init, [], [])
  AC_CHECK_PROGS(lcov, lcov)
  AC_CHECK_PROGS(genhtml, genhtml)
  AC_CHECK_PROGS(gendesc, gendesc)
  AC_CHECK_PROGS(geninfo, geninfo)
  if ! test "$lcov" || ! test "$genhtml" || ! test "$gendesc" || 
     ! test "$geninfo"; then
    AC_MSG_ERROR([Could not find lcov utilities, do you have lcov installed?])
  fi
fi

AM_CONDITIONAL(CHECK, [test "$ac_check_enabled" = yes])
AM_CONDITIONAL(DEBUG, [test "$ac_debug_enabled" = yes])
AM_CONDITIONAL(EFENCE, [test "$ac_efence_enabled" = yes])
AM_CONDITIONAL(LCOV, [test "$ac_lcov_enabled" = yes])
AM_CONDITIONAL(WARNINGS, [test "$ac_warnings_enabled" = yes])

# these allow us to append to possibly empty variables in auxdevel.am
AC_SUBST([AM_CFLAGS],[''])
AC_SUBST([LDADD],[''])
AC_SUBST([EXTRA_DIST],[''])
AC_SUBST([DISTCHECK_CONFIGURE_FLAGS],[''])
AC_SUBST([CLEANFILES],[''])
AC_SUBST([MOSTLYCLEANFILES],[''])

AC_DEFINE_UNQUOTED(BUILDVERSION, "$ac_buildversion", "Tag or version of build")

# we need to support autoconf 2.58, in which abs_builddir/abs_srcdir are not
# absolute at all
# these are also used in auxdevel.am
AC_SUBST(ac_devel_abs_builddir, `pwd -P`) # builddir is defined as '.'

AC_SANITIZE_PATH([$srcdir], ac_devel_abs_srcdir)
AC_SUBST(ac_devel_abs_srcdir)

])
