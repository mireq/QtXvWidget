#include "QtXvWidgetPlugin.h"
#include "QtXvWidget.h"

#include <QtPlugin>


QtXvWidgetPlugin::QtXvWidgetPlugin(QObject *parent) :
	QObject(parent),
	initialized(false)
{
}

bool QtXvWidgetPlugin::isContainer() const
{
	return false;
}

bool QtXvWidgetPlugin::isInitialized() const
{
	return initialized;
}

QIcon QtXvWidgetPlugin::icon() const
{
	return QIcon();
}

QString QtXvWidgetPlugin::domXml() const
{
	return "<ui language=\"c++\">\n"
	       " <widget class=\"QtXvWidget\" name=\"xvWidget\">\n"
	       "  <property name=\"geometry\">\n"
	       "   <rect>\n"
	       "    <x>0</x>\n"
	       "    <y>0</y>\n"
	       "    <width>100</width>\n"
	       "    <height>100</height>\n"
	       "   </rect>\n"
	       "  </property>\n"
	       " </widget>\n"
	        "</ui>";
}

QString QtXvWidgetPlugin::group() const
{
	return "Multimedia";
}

QString QtXvWidgetPlugin::includeFile() const
{
	return "QtXvWidget.h";
}

QString QtXvWidgetPlugin::name() const
{
	return "QtXvWidget";
}

QString QtXvWidgetPlugin::toolTip() const
{
	return "";
}

QString QtXvWidgetPlugin::whatsThis() const
{
	return "";
}

QWidget *QtXvWidgetPlugin::createWidget(QWidget *parent)
{
	return new QtXvWidget(parent);
}

void QtXvWidgetPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
	if (initialized) {
		return;
	}
	initialized = true;
}

Q_EXPORT_PLUGIN2(qtxvwidgetplugin, QtXvWidgetPlugin)
