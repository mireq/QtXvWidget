#ifndef TESTWINDOW_H_CMKJIUD2
#define TESTWINDOW_H_CMKJIUD2

#include <QMainWindow>
class QAction;
class QActionGroup;
class QMenu;
class QtXvWidget;

class TestWindow: public QMainWindow
{
Q_OBJECT
public:
	TestWindow(QWidget *parent = 0);
	~TestWindow();

private:
	void setupMenu();
	void setupAdaptors();
	bool eventFilter(QObject *obj, QEvent *event);

private slots:
	void changeAdaptor(QAction *adaptor);
	void changeFormat(QAction *format);
	void updateFormats();

private:
	QtXvWidget *m_xv;
	QMenu *m_adaptors;
	QMenu *m_formats;
	QActionGroup *m_adaptorGroup;
	QActionGroup *m_formatGroup;
}; /* -----  end of class TestWindow  ----- */

#endif /* end of include guard: TESTWINDOW_H_CMKJIUD2 */

