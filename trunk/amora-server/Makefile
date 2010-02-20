# This Makefile is an ugly hack to be able to build both applications
# in one rush and a single tar.gz containing both of them and
# necessary for packaging. -- Axel Beckert, 2009-Mar-02

VERSION=1.2
TMPDIR=.tmp
BUILDDIR=amora-server-$(VERSION)
ORIGDIR=amora-server_$(VERSION)
DIR=$(TMPDIR)/$(BUILDDIR)

all: amora-cli/amorad amora-applet/amorad-gui

amora-cli/configure: amora-cli/configure.ac
	cd amora-cli && autoreconf -i

amora-cli/Makefile: amora-cli/configure
	cd amora-cli && ./configure

amora-cli/amorad: amora-cli/Makefile
	cd amora-cli && $(MAKE)

amora-applet/Makefile:
	cd amora-applet && PKG_CONFIG_PATH=$(PKG_CONFIG_PATH):$(PWD)/amora-cli qmake-qt4

amora-applet/amorad-gui: amora-cli/amorad amora-applet/Makefile
	cd amora-applet && $(MAKE)

# Fsck. We have to build the amorad-gui, because otherwise not all
# files "make dist" in amora-applet expects, are there.
dist: amora-cli/Makefile amora-applet/Makefile amora-applet/amorad-gui
	cd amora-cli; $(MAKE) dist
	cd amora-applet; $(MAKE) dist
	mkdir -p $(DIR)
	cp -p Makefile $(DIR)
	cd $(DIR) && tar xvzf ../../amora-cli/amora-cli*.tar.gz 
	cd $(DIR) && tar xvzf ../../amora-applet/amorad-gui*.tar.gz
	mv -i $(DIR)/amorad-gui* $(DIR)/amora-applet
	mv -i $(DIR)/amora-cli-* $(DIR)/amora-cli
	cd $(TMPDIR) && tar cvzf ../$(BUILDDIR).tgz $(BUILDDIR)
	rm -rf $(TMPDIR)

# qmake includes way to many files into the tarball for easy and svn
# repository compatible use for at least building a deb, so we build a
# separate tar ball suitable for distribution and already (and for
# distinction) properly named.
debdist: amora-cli/Makefile
	cd amora-cli; $(MAKE) dist
	# Shouldn't we use "$(VERSION)" instead of "1.0.0"?
	tar cvzf amora-applet/amorad-gui-1.0.0.orig.tar.gz \
		amora-applet/*.cpp amora-applet/*.h amora-applet/*.ui \
		amora-applet/*.pro amora-applet/*.qrc amora-applet/README \
		amora-applet/images/*.png
	mkdir -p $(DIR)
	cp -p Makefile $(DIR)
	cd $(DIR) && tar xvzf ../../amora-cli/amora-cli*.tar.gz 
	cd $(DIR) && tar xvzf ../../amora-applet/amorad-gui*.orig.tar.gz
	mv -i $(DIR)/amora-cli-* $(DIR)/amora-cli
	cd $(TMPDIR) && tar cvzf ../../$(ORIGDIR).orig.tar.gz $(BUILDDIR)
	rm -rf $(TMPDIR)
	rm -v amora-applet/amorad-gui*.tar.gz
	rm -v amora-cli/amora-cli*.tar.gz

clean:
	cd amora-cli; $(MAKE) clean
	cd amora-applet; $(MAKE) clean

distclean:
	cd amora-cli; [ ! -f Makefile ] || $(MAKE) distclean
	cd amora-applet; [ ! -f Makefile ] || $(MAKE) distclean
	rm -vf amora-*/*tgz $(BUILDDIR).tgz

install: all
	cd amora-cli; $(MAKE) install
	cd amora-applet; $(MAKE) install
