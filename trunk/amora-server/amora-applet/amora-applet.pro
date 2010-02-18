TEMPLATE = app
CONFIG += qt warn_on debug link_pkgconfig

TARGET = amorad-gui
DEPENDPATH += .

isEmpty(PREFIX) {
	PREFIX = "/usr/local"
}

PKGCONFIG += libamora

# Input
FORMS = about.ui
HEADERS += amora-server.h applet.h about.h
SOURCES += amora-server.cpp applet.cpp about.cpp
RESOURCES = amora-applet.qrc

target.path = $$PREFIX/bin

INSTALLS += target
