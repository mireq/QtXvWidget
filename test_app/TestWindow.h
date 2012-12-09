#ifndef TESTWINDOW_H_CMKJIUD2
#define TESTWINDOW_H_CMKJIUD2

#include <QMainWindow>
class QAction;
class QtXvWidget;

class TestWindow: public QMainWindow
{
Q_OBJECT
public:
	TestWindow(QWidget *parent = 0);
	~TestWindow();

private:
	void setupAdaptors();

private slots:
	void changeAdaptor(QAction *adaptor);

private:
	QtXvWidget *m_xv;
}; /* -----  end of class TestWindow  ----- */

#endif /* end of include guard: TESTWINDOW_H_CMKJIUD2 */

