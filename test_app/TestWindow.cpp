#include <QAction>
#include <QMenuBar>
#include <QMetaObject>
#include <QHBoxLayout>
#include <QVideoFrame>
#include "QtXvWidget.h"
#include "QtXvControlsWidget.h"
#include "TestWindow.h"

#include <QDebug>

TestWindow::TestWindow(QWidget *parent):
	QMainWindow(parent),
	m_xv(new QtXvWidget(this))
{
	QWidget *centralWidget = new QWidget();
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(m_xv);
	layout->addWidget(new QtXvControlsWidget(m_xv));
	centralWidget->setLayout(layout);
	setCentralWidget(centralWidget);

	setupMenu();
	setupAdaptors();
}

TestWindow::~TestWindow()
{
}

void TestWindow::setupMenu()
{
	m_adaptors = menuBar()->addMenu("Adaptor");
	m_adaptorGroup = new QActionGroup(this);
	m_adaptorGroup->setExclusive(true);
	connect(m_adaptorGroup, SIGNAL(triggered(QAction*)), SLOT(changeAdaptor(QAction*)));
	m_formats = menuBar()->addMenu("Format");
	m_formatGroup = new QActionGroup(this);
	m_formatGroup->setExclusive(true);
	connect(m_formatGroup, SIGNAL(triggered(QAction*)), SLOT(changeFormat(QAction*)));
}

void TestWindow::setupAdaptors()
{
	QtXvWidget::AdaptorList adaptors = m_xv->adaptors();
	foreach (const QtXvWidget::AdaptorInfo &adaptor, adaptors) {
		QAction *action = new QAction(adaptor.name, this);
		action->setCheckable(true);
		action->setData(QVariant::fromValue<XvPortID>(adaptor.baseId));
		m_adaptorGroup->addAction(action);
		m_adaptors->addAction(action);
	}
}

void TestWindow::changeAdaptor(QAction *adaptor)
{
	m_xv->setAdaptor(adaptor->data().value<XvPortID>());
	updateFormats();
}

void TestWindow::changeFormat(QAction *format)
{
	m_xv->setFormat(format->data().value<int>());
}

void TestWindow::updateFormats()
{
	while (m_formats->actions().count()) {
		m_formatGroup->removeAction(m_formats->actions().first());
		m_formats->removeAction(m_formats->actions().first());
	}

	QtXvWidget::FormatList formats = m_xv->formats();
	foreach (const QtXvWidget::PixelFormat &format, formats) {
		QAction *action = new QAction(format.name(), this);
		action->setCheckable(true);
		if (format.id == m_xv->format()) {
			action->setChecked(true);
		}
		action->setData(QVariant::fromValue<int>(format.id));
		m_formatGroup->addAction(action);
		m_formats->addAction(action);
	}
}

