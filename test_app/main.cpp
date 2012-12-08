#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include "TestWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	TestWindow win;
	win.show();
	return app.exec();
}
