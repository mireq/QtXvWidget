CONFIG += designer plugin
TARGET = qtxv
TEMPLATE = lib
LIBS += -lXv\
	-lX11
DEFINES += QTXV_LIBRARY
QT += core gui multimedia
contains(QT_VERSION, ^5\\..*) {
	QT+= gui-private widgets
}
SOURCES += QtXvWidget.cpp\
	QtXvControlsWidget.cpp\
	QtXvTestImage.cpp\
	QtXvWidgetPlugin.cpp\

HEADERS += QtXvWidget.h\
	QtXvControlsWidget.h\
	QtXvTestImage.h\
	QtXvWidgetPlugin.h

unix:!symbian {
	target.path = /usr/lib/qt4/plugins/designer
	INSTALLS += target
}
