#include <QApplication>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QWindow>
#include <qpa/qplatformnativeinterface.h>
#endif
#include "QtXvWidget.h"


struct RgbFormatInfo {
	QVideoFrame::PixelFormat pixelFormat;

	int bitsPerPixel;
	int format;
	int numPlanes;

	int depth;
	unsigned int redMask;
	unsigned int greenMask;
	unsigned int blueMask;
};

bool operator==(const XvImageFormatValues &format, const RgbFormatInfo &rgb)
{
	return
		format.type           == XvRGB &&
		format.bits_per_pixel == rgb.bitsPerPixel &&
		format.format         == rgb.format &&
		format.depth          == rgb.depth &&
		format.red_mask       == rgb.redMask &&
		format.green_mask     == rgb.greenMask &&
		format.blue_mask      == rgb.blueMask;
}

static const RgbFormatInfo rgb_formatsLookup[] =
{
	{ QVideoFrame::Format_ARGB32, 32, XvPacked, 1, 32, 0x00FF0000, 0x0000FF00, 0x000000FF },
	{ QVideoFrame::Format_RGB32,  32, XvPacked, 1, 24, 0x00FF0000, 0x0000FF00, 0x000000FF },
	{ QVideoFrame::Format_RGB24,  24, XvPacked, 1, 24, 0x00FF0000, 0x0000FF00, 0x000000FF },
	{ QVideoFrame::Format_RGB565, 16, XvPacked, 1, 16, 0x0000F800, 0x000007E0, 0x0000001F },
	{ QVideoFrame::Format_BGRA32, 32, XvPacked, 1, 32, 0xFF000000, 0x00FF0000, 0x0000FF00 },
};

struct YuvFormatInfo {
	QVideoFrame::PixelFormat pixelFormat;

	int bitsPerPixel;
	int format;
	int numPlanes;

	unsigned int ySampleBits;
	unsigned int uSampleBits;
	unsigned int vSampleBits;
	unsigned int horzYPeriod;
	unsigned int horzUPeriod;
	unsigned int horzVPeriod;
	unsigned int vertYPeriod;
	unsigned int vertUPeriod;
	unsigned int vertVPeriod;
	char componentOrder[32];
};

bool operator==(const XvImageFormatValues &format, const YuvFormatInfo &yuv)
{
	return
		format.type           == XvYUV &&
		format.bits_per_pixel == yuv.bitsPerPixel &&
		format.format         == yuv.format &&
		format.num_planes     == yuv.numPlanes &&
		format.y_sample_bits  == yuv.ySampleBits &&
		format.u_sample_bits  == yuv.uSampleBits &&
		format.v_sample_bits  == yuv.vSampleBits &&
		format.horz_y_period  == yuv.horzYPeriod &&
		format.horz_u_period  == yuv.horzUPeriod &&
		format.horz_v_period  == yuv.horzVPeriod &&
		format.vert_y_period  == yuv.vertYPeriod &&
		format.vert_u_period  == yuv.vertUPeriod &&
		format.vert_v_period  == yuv.vertVPeriod &&
		qstrncmp(format.component_order, yuv.componentOrder, 32) == 0;
}

static const YuvFormatInfo yuv_formatsLookup[] =
{
	{ QVideoFrame::Format_YUV444 , 24, XvPacked, 1, 8, 8, 8, 1, 1, 1, 1, 1, 1, "YUV"  },
	{ QVideoFrame::Format_YUV420P, 12, XvPlanar, 3, 8, 8, 8, 1, 2, 2, 1, 2, 2, "YUV"  },
	{ QVideoFrame::Format_YV12,    12, XvPlanar, 3, 8, 8, 8, 1, 2, 2, 1, 2, 2, "YVU"  },
	{ QVideoFrame::Format_UYVY,    16, XvPacked, 1, 8, 8, 8, 1, 2, 2, 1, 1, 1, "UYVY" },
	{ QVideoFrame::Format_YUYV,    16, XvPacked, 1, 8, 8, 8, 1, 2, 2, 1, 1, 1, "YUY2" },
	{ QVideoFrame::Format_YUYV,    16, XvPacked, 1, 8, 8, 8, 1, 2, 2, 1, 1, 1, "YUYV" },
	{ QVideoFrame::Format_NV12,    12, XvPlanar, 2, 8, 8, 8, 1, 2, 2, 1, 2, 2, "YUV"  },
	{ QVideoFrame::Format_NV12,    12, XvPlanar, 2, 8, 8, 8, 1, 2, 2, 1, 2, 2, "YVU"  },
	{ QVideoFrame::Format_Y8,      8 , XvPlanar, 1, 8, 0, 0, 1, 0, 0, 1, 0, 0, "Y"    },
};

QString QtXvWidget::PixelFormatInfo::name() const
{
	switch (format) {
		case QVideoFrame::Format_ARGB32:
			return "ARGB32";
		case QVideoFrame::Format_RGB32:
			return "RGB32";
		case QVideoFrame::Format_RGB24:
			return "RGB24";
		case QVideoFrame::Format_RGB565:
			return "RGB565";
		case QVideoFrame::Format_BGRA32:
			return "BGRA32";
		case QVideoFrame::Format_YUV444:
			return "YUV444";
		case QVideoFrame::Format_YUV420P:
			return "YUV420P";
		case QVideoFrame::Format_YV12:
			return "YV12";
		case QVideoFrame::Format_UYVY:
			return "UYVY";
		case QVideoFrame::Format_YUYV:
			return "YUYV";
		case QVideoFrame::Format_NV12:
			return "NV12";
		case QVideoFrame::Format_Y8:
			return "Y8";
		default:
			return "?";
	}
}

QtXvWidget::QtXvWidget(QWidget *parent):
	QWidget(parent),
	m_port(0),
	m_format(0)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	if (!hasXvExtension()) {
		return;
	}
}

QtXvWidget::~QtXvWidget()
{
	ungrabPort();
}

QtXvWidget::AdaptorList QtXvWidget::adaptors() const
{
	AdaptorList list;
	if (!hasXvExtension()) {
		return list;
	}

	XvAdaptorInfo *adaptorInfo = 0;
	unsigned int count = 0;
	if (XvQueryAdaptors(getDpy(), DefaultRootWindow(getDpy()), &count, &adaptorInfo) != Success) {
		qCritical("Could not get adaptors!");
		return list;
	}

	for (unsigned int adaptor = 0; adaptor < count; ++adaptor) {
		AdaptorInfo info;
		XvAdaptorInfo *current = adaptorInfo + adaptor;
		if ((current->type & XvInputMask) && (current->type & XvImageMask)) {
			info.baseId = current->base_id;
			info.name = QString(current->name);
			info.numPorts = current->num_ports;
			list.append(info);
		}
	}

	return list;
}

QtXvWidget::PixelFormatList QtXvWidget::formats() const
{
	return m_formats;
}

QtXvWidget::AttributeList QtXvWidget::xvAttributes() const
{
	AttributeList retAttributes;
	if (!isInitialized()) {
		return retAttributes;
	}

	int count = 0;
	XvAttribute *attributes = XvQueryPortAttributes(getDpy(), m_port, &count);
	if (!attributes) {
		return retAttributes;
	}

	for (int i = 0; i < count; ++i) {
		XvAttribute attr = attributes[i];
		AttributeFlags flags = NoFlag;
		if (attr.flags & XvGettable) {
			flags |= ReadFlag;
		}
		if (attr.flags & XvSettable) {
			flags |= WriteFlag;
		}
		AttributeInfo info = { attr.name, attr.min_value, attr.max_value, flags };
		retAttributes.append(info);
	}

	XFree(attributes);
	return retAttributes;
}

bool QtXvWidget::isInitialized() const
{
	return m_port != 0;
}

bool QtXvWidget::setAdaptor(XvPortID baseId)
{
	if (!hasXvExtension()) {
		return false;
	}
	ungrabPort();
	foreach (const AdaptorInfo &adaptor, adaptors()) {
		if (adaptor.baseId == baseId) {
			return setAdaptor(adaptor);
		}
	}
	qCritical("Bad base ID");
	return false;
}

bool QtXvWidget::setAdaptor(const AdaptorInfo &adaptor)
{
	for (XvPortID port = adaptor.baseId; port < (adaptor.baseId + adaptor.numPorts); ++port) {
		if (XvGrabPort(getDpy(), port, CurrentTime) == Success) {
			m_port = port;
			updateFormats();
			emit initializedChanged();
			return true;
		}
	}
	qCritical("No suitable port");
	return false;
}

int QtXvWidget::format() const
{
	return m_format;
}

bool QtXvWidget::setFormat(int formatId)
{
	foreach (const PixelFormatInfo &format, m_formats) {
		if (format.id == formatId) {
			setFormat(format);
			return true;
		}
	}
	qCritical("Could not find format!");
	return false;
}

QVideoFrame::PixelFormat QtXvWidget::pixelFormat() const
{
	foreach (const PixelFormatInfo &format, m_formats) {
		if (format.id == m_format) {
			return format.format;
		}
	}
	return QVideoFrame::Format_Invalid;
}

bool QtXvWidget::setPixelFormat(QVideoFrame::PixelFormat format)
{
	foreach (const PixelFormatInfo &formatInfo, m_formats) {
		if (formatInfo.format == format) {
			setFormat(formatInfo);
			return true;
		}
	}
	qCritical("Could not find format!");
	return false;
}

void QtXvWidget::setXvAttribute(const QString &attribute, int value)
{
	if (!isInitialized()) {
		return;
	}

	Atom atom = XInternAtom(getDpy(), attribute.toStdString().c_str(), True);
	XvSetPortAttribute(getDpy(), m_port, atom, value);
	present(m_frame);
}

int QtXvWidget::getXvAttribute(const QString &attribute) const
{
	if (m_port == 0) {
		return 0;
	}

	Atom atom = XInternAtom(getDpy(), attribute.toStdString().c_str(), True);
	int value = 0;
	XvGetPortAttribute(getDpy(), m_port, atom, &value);
	return value;
}

bool QtXvWidget::present(const QVideoFrame &frame)
{
	m_frame = frame;
	if (!isInitialized() || frame.pixelFormat() != pixelFormat() || frame.bits() == 0) {
		return false;
	}

	char *bits = const_cast<char *>(reinterpret_cast<const char *>(frame.bits()));
	XvImage *image = XvCreateImage(getDpy(), m_port, m_format, bits, frame.width(), frame.height());
	XGCValues xgcv;
	GC gc = XCreateGC(getDpy(), winId(), 0L, &xgcv);
	XvPutImage(getDpy(), m_port, winId(), gc, image, 0, 0, image->width, image->height, 0, 0, width(), height());
	XFreeGC(getDpy(), gc);
	XFree(image);

	return true;
}

bool QtXvWidget::event(QEvent *event)
{
	if (event->type() == QEvent::Resize || event->type() == QEvent::Paint || event->type() == QEvent::Move) {
		present(m_frame);
		return QWidget::event(event);
	}
	return QWidget::event(event);
}

Display *QtXvWidget::getDpy() const
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	QWindow *window = new QWindow(/* screen */);
	Display *dpy = static_cast<Display *>(qGuiApp->platformNativeInterface()->nativeResourceForWindow("display", window));
#else
	Display *dpy = x11Info().display();
#endif
	if (!dpy) {
		qCritical("Could not open display!");
	}
	return dpy;
}

bool QtXvWidget::hasXvExtension() const
{
	Display *dpy = getDpy();
	if (!dpy) {
		return false;
	}
	unsigned int version, release, request_base, event_base, error_base;
	if (XvQueryExtension(dpy, &version, &release, &request_base, &event_base, &error_base) == Success) {
		return true;
	}
	else {
		return false;
	}
}

void QtXvWidget::ungrabPort()
{
	clearFormat();
	if (!isInitialized()) {
		return;
	}
	XvUngrabPort(getDpy(), m_port, CurrentTime);
	m_port = 0;
	emit initializedChanged();
}

void QtXvWidget::updateFormats()
{
	clearFormat();
	int count = 0;
	XvImageFormatValues *formats = XvListImageFormats(getDpy(), m_port, &count);
	if (formats) {
		for (int i = 0; i < count; ++i) {
			if (formats[i].type == XvRGB) {
				for (unsigned int j = 0; j < sizeof(rgb_formatsLookup) / sizeof(RgbFormatInfo); ++j) {
					if (formats[i] == rgb_formatsLookup[j]) {
						m_formats.append(PixelFormatInfo(formats[i].id, rgb_formatsLookup[j].pixelFormat));
					}
				}
			}
			else if (formats[i].type == XvYUV) {
				for (unsigned int j = 0; j < sizeof(yuv_formatsLookup) / sizeof(YuvFormatInfo); ++j) {
					if (formats[i] == yuv_formatsLookup[j]) {
						m_formats.append(PixelFormatInfo(formats[i].id, yuv_formatsLookup[j].pixelFormat));
					}
				}
			}
		}
		XFree(formats);
		if (m_formats.count()) {
			setFormat(m_formats.first().id);
		}
	}
}

void QtXvWidget::setFormat(const PixelFormatInfo &format)
{
	m_format = format.id;

	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NoSystemBackground, true);
}

void QtXvWidget::clearFormat()
{
	m_format = 0;
	m_formats.clear();

	setAttribute(Qt::WA_PaintOnScreen, false);
	setAttribute(Qt::WA_NoSystemBackground, false);
}

