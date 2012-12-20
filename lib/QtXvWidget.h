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
	struct FormatInfo {
		QVideoFrame::PixelFormat pixelFormat;

		int bitsPerPixel;
		int format;
		int numPlanes;

		// RGB
		int depth;
		unsigned int redMask;
		unsigned int greenMask;
		unsigned int blueMask;

		// YUV
		unsigned int ySampleBits;
		unsigned int uSampleBits;
		unsigned int vSampleBits;
		unsigned int horzYPeriod;
		unsigned int horzUPeriod;
		unsigned int horzVPeriod;
		unsigned int vertYPeriod;
		unsigned int vertUPeriod;
		unsigned int vertVPeriod;
	};
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

	void setBorder(int top, int right, int bottom, int left);

	int format() const;
	bool setFormat(int formatId);
	QVideoFrame::PixelFormat pixelFormat() const;
	bool setPixelFormat(QVideoFrame::PixelFormat format);
	FormatInfo formatInfo() const;

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
	int m_topBorder;
	int m_rightBorder;
	int m_bottomBorder;
	int m_leftBorder;

	XvPortID m_port;
	int m_format;
	PixelFormatList m_formats;
	QVideoFrame m_frame;
};

#endif // QTXVWIDGET_H
