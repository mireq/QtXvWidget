TARGET = xv-test
TEMPLATE = app
LIBS += -L../lib -lqtxv
INCLUDEPATH += ../lib
QT += core gui multimedia
contains(QT_VERSION, ^5\\..*) {
	QT+= widgets
}

SOURCES += TestWindow.cpp\
	main.cpp

HEADERS += TestWindow.h
