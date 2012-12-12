CONFIG += designer plugin
TARGET = qtxv
TEMPLATE = lib
LIBS += -lXv\
	-lX11
DEFINES += QTXV_LIBRARY
QT += multimedia
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
