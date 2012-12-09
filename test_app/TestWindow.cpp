#include <QAction>
#include <QMenuBar>
#include "QtXvWidget.h"
#include "TestWindow.h"

#include <QDebug>

TestWindow::TestWindow(QWidget *parent):
	QMainWindow(parent),
	m_xv(new QtXvWidget(this))
{
	setCentralWidget(m_xv);
	setupAdaptors();
}

TestWindow::~TestWindow()
{
}

void TestWindow::setupAdaptors()
{
	QMenu *adaptorMenu = menuBar()->addMenu("Adaptor");
	QActionGroup *adaptorGroup = new QActionGroup(this);
	adaptorGroup->setExclusive(true);
	QtXvWidget::AdaptorList adaptors = m_xv->getAdaptors();
	foreach (const QtXvWidget::AdaptorInfo &adaptor, adaptors) {
		QAction *action = new QAction(adaptor.name, this);
		action->setCheckable(true);
		action->setData(adaptor.baseId);
		adaptorGroup->addAction(action);
		adaptorMenu->addAction(action);
	}
	connect(adaptorGroup, SIGNAL(triggered(QAction*)), SLOT(changeAdaptor(QAction*)));
}

void TestWindow::changeAdaptor(QAction *adaptor)
{
	m_xv->setAdaptor(adaptor->data());
}

