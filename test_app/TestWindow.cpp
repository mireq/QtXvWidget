#include "QtXvWidget.h"
#include "TestWindow.h"

TestWindow::TestWindow(QWidget *parent):
	QMainWindow(parent)
{
	setCentralWidget(new QtXvWidget(this));
}

TestWindow::~TestWindow()
{
}

