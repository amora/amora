# (C) 2005-2006 Mandriva Conectiva S.A.
# (C) 2006 Ademar de Souza Reis Jr. <ademar@ademar.org>
#
# Based on sniffdet configure.ac: http://sniffdet.sourceforge.net
# Licensed under GNU-GPL

## Please note that prog_CFLAGS and prog_LDADD overwrite AM_CFLAGS and LDADD, set
## by the included file mk/auxdevel.am.  If you want to use program-specific
## flags, do something like amora_CFLAGS = -march=athlon $(AM_CFLAGS)

AUTOMAKE_OPTIONS = foreign

# amora itself
bin_PROGRAMS = amorad
amorad_SOURCES = $(csourcedir)/amora.c \
                 $(csourcedir)/protocol.h $(csourcedir)/protocol.c \
                 $(csourcedir)/x11_event.h $(csourcedir)/x11_event.c \
                 $(csourcedir)/bluetooth-frontend.c \
                 $(csourcedir)/bluetooth-wrapper.h $(csourcedir)/bluetooth-bluez.c \
                 $(csourcedir)/amora-log.h $(csourcedir)/amora-log.c \
                 $(csourcedir)/amora-loop.h $(csourcedir)/amora-loop.c \
                 $(csourcedir)/imscreen.h $(csourcedir)/imscreen.c

if HAVE_DBUS
amorad_SOURCES += $(csourcedir)/amora-dbus.h $(csourcedir)/amora-dbus.c \
                  $(csourcedir)/bluetooth-bluez-dbus.h $(csourcedir)/bluetooth-bluez-dbus.c
endif

amorad_LDADD = $(LIBBLUEZ_LIBS) $(LIBX11_LIBS) $(LIBXTST_LIBS) $(LIBIMLIB2_LIBS) $(LIBDBUS_LIBS)
amorad_CFLAGS = $(AM_CFLAGS) $(LIBBLUEZ_CFLAGS) $(LIBX11_CFLAGS) $(LIBXTST_CFLAGS) $(LIBIMLIB2_CFLAGS) $(LIBDBUS_CFLAGS)

amoramandir = $(mandir)/man8
amoraman_DATA = amorad.8

# unit tests
if CHECK
TESTS = utest
noinst_PROGRAMS = utest
utest_SOURCES = $(utestdir)/utest.c \
                $(utestdir)/utest_libfake.c \
                $(utestdir)/utest_amora-log.c \
                $(utestdir)/utest_amora-log.h \
                $(utestdir)/utest_amora-loop.c \
                $(utestdir)/utest_amora-loop.h

if HAVE_DBUS
utest_SOURCES += $(utestdir)/utest_amora-dbus.h $(utestdir)/utest_amora-dbus.c
endif

utest_CPPFLAGS = $(CHECK_FLAGS) $(AM_CPPFLAGS) -I$(csourcedir) $(LIBDBUS_CFLAGS)
utest_LDADD = $(CHECK_LIBS) $(LDADD) $(LIBDBUS_LIBS)
endif

# by now, amora manpage points to the amorad one
install-data-hook:
	mkdir -p $(DESTDIR)$(mandir)/man7/
	ln -sf ../man8/amorad.8 $(DESTDIR)$(mandir)/man7/amora.7

uninstall-hook:
	rm $(DESTDIR)$(mandir)/man7/amora.7

EXTRA_DIST = $(srcdir)/m4/auxdevel.m4 \
             $(srcdir)/m4/check.m4 \
             $(srcdir)/m4/define_dirs.m4 \
             $(srcdir)/mk/auxdevel.am \
             $(srcdir)/mk/libtool-fix.am \
             $(srcdir)/README \
             $(srcdir)/COPYING \
             Doxyfile.in \
             amora-server.spec \
             amorad.8


DISTCHECK_CONFIGURE_FLAGS = --enable-efence --enable-debug \
                            --enable-warnings --disable-check

# this variable is scanned by autoreconf and needs to be static (no expansion)
ACLOCAL_AMFLAGS = -I m4

include $(srcdir)/mk/auxdevel.am

# vim: set noet tw=74:
