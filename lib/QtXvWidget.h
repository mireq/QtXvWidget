#ifndef QTXVWIDGET_H
#define QTXVWIDGET_H

#include <QList>
#include <QPair>
#include <QVariant>
#include <QWidget>
#include <QX11Info>
#include <QtDesigner/QDesignerExportWidget>

/*QDESIGNER_WIDGET_EXPORT */
class QtXvWidget: public QWidget
{
Q_OBJECT
public:
	struct AdaptorInfo {
		QVariant baseId;
		QString name;
		unsigned long numPorts;
		char type;
	};
	typedef QList<AdaptorInfo> AdaptorList;
	QtXvWidget(QWidget *parent = 0);
	~QtXvWidget();
	AdaptorList getAdaptors() const;
	void setAdaptor(QVariant baseId);

private:
	Display *getDpy() const;
	bool hasXvExtension() const;

private:
	bool m_xvInitialized;
};

#endif // QTXVWIDGET_H
