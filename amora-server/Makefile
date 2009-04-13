# This Makefile is an ugly hack to be able to build both applications
# in one rush and a single tar.gz containing both of them and
# necessary for packaging. -- Axel Beckert, 2009-Mar-02

VERSION=1.2
TMPDIR=.tmp
BUILDDIR=amora-server-$(VERSION)
DIR=$(TMPDIR)/$(BUILDDIR)

all: amora-cli/amorad amora-applet/amorad-gui

amora-cli/Makefile:
	cd amora-cli && autoreconf -i && ./configure

amora-cli/amorad: amora-cli/Makefile
	cd amora-cli && $(MAKE)

amora-applet/Makefile:
	cd amora-applet && qmake-qt4
	sed -e's:^INCPATH *=.*$$:& -I../amora-cli/src/:' -i amora-applet/Makefile
	sed -e's:^LIBS *=.*$$:& -L../amora-cli/.libs/:' -i amora-applet/Makefile

amora-applet/amorad-gui: amora-cli/amorad amora-applet/Makefile
	cd amora-applet && $(MAKE)

# Fsck. We have to build the amorad-gui, because otherwise not all
# files "make dist" in amora-applet expects, are there.
dist: amora-cli/Makefile amora-applet/Makefile amora-applet/amorad-gui
	cd amora-cli; $(MAKE) dist
	cd amora-applet; $(MAKE) dist
	mkdir -p .tmp/amora-server-$(VERSION)
	cp -p Makefile $(DIR)
	cd $(DIR) && tar xvzf ../../amora-cli/amora-server*.tar.gz 
	cd $(DIR) && tar xvzf ../../amora-applet/amorad-gui*.tar.gz
	cd $(TMPDIR) && tar cvzf ../$(BUILDDIR).tgz $(BUILDDIR)
	rm -rf $(TMPDIR)

clean:
	cd amora-cli; $(MAKE) clean
	cd amora-applet; $(MAKE) clean

distclean:
	cd amora-cli; $(MAKE) distclean
	cd amora-applet; $(MAKE) distclean