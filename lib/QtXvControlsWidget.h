#ifndef QTXVCONTROLSWIDGET_H_T8VEDY9G
#define QTXVCONTROLSWIDGET_H_T8VEDY9G

#include "QtXvWidget.h"
class QGridLayout;
class QSignalMapper;


class QtXvControlsWidget: public QWidget
{
Q_OBJECT
public:
	QtXvControlsWidget(QtXvWidget *widget, QWidget *parent = 0);
	~QtXvControlsWidget();

private slots:
	void updateControls();
	void valueChanged(const QString &attribute);

private:
	QtXvWidget *m_xv;
	QGridLayout *m_layout;
	QSignalMapper *m_mapper;
}; /* -----  end of class QtXvControlsWidget  ----- */

#endif /* end of include guard: QTXVCONTROLSWIDGET_H_T8VEDY9G */

