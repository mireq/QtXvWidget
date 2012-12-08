#ifndef QTXVWIDGET_H
#define QTXVWIDGET_H

#include <QWidget>
#include <QX11Info>
#include <QtDesigner/QDesignerExportWidget>

/*QDESIGNER_WIDGET_EXPORT */
class QtXvWidget: public QWidget
{
Q_OBJECT
public:
	QtXvWidget(QWidget *parent = 0);
	~QtXvWidget();

private:
	Display *getDpy() const;
	bool hasXvExtension() const;

	bool m_xvInitialized;
};

#endif // QTXVWIDGET_H
