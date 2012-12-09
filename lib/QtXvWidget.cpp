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

QtXvWidget::AdaptorList QtXvWidget::getAdaptors() const
{
	AdaptorList list;
	if (!m_xvInitialized) {
		return list;
	}

	XvAdaptorInfo *adaptorInfo = 0;
	unsigned int count = 0;
	if (XvQueryAdaptors(getDpy(), DefaultRootWindow(getDpy()), &count, &adaptorInfo) != Success) {
		qCritical("Could not get adaptors!");
		return list;
	}

	for (unsigned int adaptor = 0; adaptor < count; ++adaptor) {
		AdaptorInfo info;
		XvAdaptorInfo *current = adaptorInfo + adaptor;
		info.baseId = QVariant::fromValue<qlonglong>(current->base_id);
		info.name = QString(current->name);
		info.numPorts = current->num_ports;
		list.append(info);
	}

	return list;
}

void QtXvWidget::setAdaptor(QVariant baseId)
{
	AdaptorList adaptors = getAdaptors();
	foreach (const AdaptorInfo &adaptor, adaptors) {
		if (adaptor.baseId == baseId) {
			return;
		}
	}
	qCritical("Bad base ID");
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

