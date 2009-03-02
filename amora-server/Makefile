# This Makefile is an ugly hack to be able to build both applications
# in one rush and a single tar.gz containing both of them and
# necessary for packaging. -- Axel Beckert, 2009-Mar-02

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


dist: amora-cli/Makefile amora-applet/Makefile
	cd amora-cli; $(MAKE) dist
	cd amora-applet; $(MAKE) dist
	mkdir -p .tmp/amora-server
	cd .tmp/amora-server && tar xvzf ../../amora-cli/amora-server*.tar.gz 
	cd .tmp/amora-server && tar xvzf ../../amora-applet/amorad-gui*.tar.gz
	cd .tmp && tar cvzf ../amora-server.tgz amora-server
	rm -rf .tmp/

clean:
	cd amora-cli; $(MAKE) clean
	cd amora-applet; $(MAKE) clean

distclean:
	cd amora-cli; $(MAKE) distclean
	cd amora-applet; $(MAKE) distclean