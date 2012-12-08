#include "QtXvWidget.h"
#include <QDebug>


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>


QtXvWidget::QtXvWidget(QWidget *parent):
	QWidget(parent),
	m_xvInitialized(false)
{
	if (!getDpy() || !hasXvExtension()) {
		return;
	}
	m_xvInitialized = true;
}

QtXvWidget::~QtXvWidget()
{
}

Display *QtXvWidget::getDpy() const
{
	Display *dpy = x11Info().display();
	if (!dpy) {
		qCritical("Could not open display!");
	}
	return dpy;
}

bool QtXvWidget::hasXvExtension() const
{
	unsigned int version, release, request_base, event_base, error_base;
	if (XvQueryExtension(getDpy(), &version, &release, &request_base, &event_base, &error_base) == Success) {
		qDebug() << "X-Video Extension" << (QString::number(version) + "." + QString::number(release)).toStdString().c_str();
		return true;
	}
	else {
		return false;
	}
}

