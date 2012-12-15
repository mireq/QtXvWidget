#ifndef QTXVWIDGET_H
#define QTXVWIDGET_H

#include <QDebug>
#include <QList>
#include <QPair>
#include <QVariant>
#include <QVideoFrame>
#include <QWidget>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QX11Info>
#endif
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
	struct PixelFormatInfo {
		PixelFormatInfo() {};
		PixelFormatInfo(int id, const QVideoFrame::PixelFormat &format): id(id), format(format) {};
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
	typedef QList<PixelFormatInfo> PixelFormatList;
	typedef QList<AttributeInfo> AttributeList;

	QtXvWidget(QWidget *parent = 0);
	~QtXvWidget();

	AdaptorList adaptors() const;
	PixelFormatList formats() const;
	AttributeList xvAttributes() const;

	bool isInitialized() const;

	bool setAdaptor(XvPortID baseId);
	bool setAdaptor(const AdaptorInfo &adaptor);

	int format() const;
	bool setFormat(int formatId);
	QVideoFrame::PixelFormat pixelFormat() const;
	bool setPixelFormat(QVideoFrame::PixelFormat format);

	void setXvAttribute(const QString &attribute, int value);
	int getXvAttribute(const QString &attribute) const;

	bool present(const QVideoFrame &frame);

signals:
	void initializedChanged();

protected:
	bool event(QEvent *event);

private:
	Display *getDpy() const;
	bool hasXvExtension() const;
	void ungrabPort();
	void updateFormats();
	void setFormat(const PixelFormatInfo &format);
	void clearFormat();

private:
	XvPortID m_port;
	int m_format;
	PixelFormatList m_formats;
	QVideoFrame m_frame;
};

#endif // QTXVWIDGET_H
