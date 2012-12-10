#ifndef QTXVWIDGET_H
#define QTXVWIDGET_H

#include <QDebug>
#include <QList>
#include <QPair>
#include <QVariant>
#include <QVideoFrame>
#include <QWidget>
#include <QX11Info>
#include <QtDesigner/QDesignerExportWidget>


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>


/*QDESIGNER_WIDGET_EXPORT */
class QtXvWidget: public QWidget
{
Q_OBJECT
public:
	struct AdaptorInfo {
		XvPortID baseId;
		QString name;
		unsigned long numPorts;
		char type;
	};
	struct PixelFormat {
		PixelFormat() {};
		PixelFormat(int id, const QVideoFrame::PixelFormat &format): id(id), format(format) {};
		int id;
		QVideoFrame::PixelFormat format;
		QString name() const;
	};
	enum AttributeFlag {
		NoFlag = 0x00,
		ReadFlag = 0x01,
		WriteFlag = 0x02
	};
	Q_DECLARE_FLAGS(AttributeFlags, AttributeFlag)
	struct AttributeInfo {
		QString name;
		int min;
		int max;
		AttributeFlags flags;
	};
	typedef QList<AdaptorInfo> AdaptorList;
	typedef QList<PixelFormat> FormatList;
	typedef QList<AttributeInfo> AttributeList;

	QtXvWidget(QWidget *parent = 0);
	~QtXvWidget();
	AdaptorList adaptors() const;
	FormatList formats() const;
	void setAdaptor(XvPortID baseId);
	void setFormat(int formatId);
	int format() const;
	XvPortID port() const;
	AttributeList attributes() const;
	void setAttribute(const QString &attribute, int value);
	int getAttribute(const QString &attribute) const;

signals:
	void portChanged(XvPortID port);

private:
	Display *getDpy() const;
	bool hasXvExtension() const;
	void ungrabPort();
	void updateFormats();

private:
	bool m_xvInitialized;
	XvPortID m_port;
	int m_format;
	FormatList m_formats;
};

#endif // QTXVWIDGET_H
