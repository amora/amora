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
bin_PROGRAMS = amora
amora_SOURCES = $(csourcedir)/main.c \
                   $(csourcedir)/protocol.h $(csourcedir)/protocol.c \
                   $(csourcedir)/x11_event.h $(csourcedir)/x11_event.c \
                   $(csourcedir)/bluecode.h $(csourcedir)/linux.c \
                   $(csourcedir)/log.h $(csourcedir)/log.c \
		   $(csourcedir)/imscreen.h $(csourcedir)/imscreen.c

amora_LDADD = $(LIBBLUEZ_LIBS) $(LIBX11_LIBS) $(LIBXTST_LIBS) $(LIBIMLIB2_LIBS)
amora_CFLAGS = $(AM_CFLAGS) $(LIBBLUEZ_CFLAGS) $(LIBX11_CFLAGS) $(LIBXTST_CFLAGS) $(LIBIMLIB2_CFLAGS)

EXTRA_DIST = $(srcdir)/m4/auxdevel.m4 \
             $(srcdir)/m4/check.m4 \
             $(srcdir)/m4/acx_pthread.m4 \
             $(srcdir)/m4/define_dirs.m4 \
             $(srcdir)/mk/auxdevel.am \
             $(srcdir)/mk/libtool-fix.am \
             $(srcdir)/README \
             $(srcdir)/COPYING \
             Doxyfile.in


DISTCHECK_CONFIGURE_FLAGS = --enable-efence --enable-debug \
                            --enable-warnings --disable-check

# this variable is scanned by autoreconf and needs to be static (no expansion)
ACLOCAL_AMFLAGS = -I m4

include $(srcdir)/mk/auxdevel.am

# vim: set et tw=74:
