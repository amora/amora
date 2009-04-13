TEMPLATE = app
TARGET = amorad-gui
DEPENDPATH += .
INCLUDEPATH += . ../amora-cli/src
FORMS = about.ui
CONFIG += warn_on debug
LIBS = -L../amora-cli/.libs -lamora -lbluetooth -lImlib2 -lX11 -lXtst -ldbus-1
QMAKE_CXXFLAGS += -Wshadow
RESOURCES = amora-applet.qrc

# Input
HEADERS += amora-server.h applet.h about.h
SOURCES += amora-server.cpp applet.cpp about.cpp
