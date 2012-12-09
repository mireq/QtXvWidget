TARGET = xv-test
TEMPLATE = app
LIBS += -L../lib -lqtxv
INCLUDEPATH += ../lib
QT += multimedia

SOURCES += TestWindow.cpp\
	main.cpp

HEADERS += TestWindow.h
