TARGET = xv-test
TEMPLATE = app
LIBS += -L../lib -lqtxv
INCLUDEPATH += ../lib

SOURCES += TestWindow.cpp\
	main.cpp

HEADERS += TestWindow.h
