#include <QAction>
#include <QEvent>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMetaObject>
#include <QResizeEvent>
#include <QVideoFrame>
#include "QtXvControlsWidget.h"
#include "QtXvTestImage.h"
#include "QtXvWidget.h"
#include "TestWindow.h"

#include <QDebug>

TestWindow::TestWindow(QWidget *parent):
	QMainWindow(parent),
	m_xv(new QtXvWidget(this))
{
	QWidget *centralWidget = new QWidget();
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(m_xv);
	layout->addWidget(new QtXvControlsWidget(m_xv));
	centralWidget->setLayout(layout);
	setCentralWidget(centralWidget);

	setupMenu();
	setupAdaptors();

	m_xv->installEventFilter(this);
}

TestWindow::~TestWindow()
{
}

void TestWindow::setupMenu()
{
	m_adaptors = menuBar()->addMenu("Adaptor");
	m_adaptorGroup = new QActionGroup(this);
	m_adaptorGroup->setExclusive(true);
	connect(m_adaptorGroup, SIGNAL(triggered(QAction*)), SLOT(changeAdaptor(QAction*)));
	m_formats = menuBar()->addMenu("Format");
	m_formatGroup = new QActionGroup(this);
	m_formatGroup->setExclusive(true);
	connect(m_formatGroup, SIGNAL(triggered(QAction*)), SLOT(changeFormat(QAction*)));
}

void TestWindow::setupAdaptors()
{
	QtXvWidget::AdaptorList adaptors = m_xv->adaptors();
	foreach (const QtXvWidget::AdaptorInfo &adaptor, adaptors) {
		QAction *action = new QAction(adaptor.name, this);
		action->setCheckable(true);
		action->setData(QVariant::fromValue<XvPortID>(adaptor.baseId));
		m_adaptorGroup->addAction(action);
		m_adaptors->addAction(action);
	}
}

bool TestWindow::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::Resize) {
		QSize size = static_cast<QResizeEvent *>(event)->size();
		int w = size.width();
		int h = size.height();
		int lineLen = w;
		QVideoFrame::PixelFormat fmt = m_xv->pixelFormat();
		if (w == 0 || h == 0 || fmt == QVideoFrame::Format_Invalid) {
			return false;
		}
		QVideoFrame frame;
		QImage img = QtXvTestImage().generateImage(w, h);
		if (fmt == QVideoFrame::Format_YUYV) {
			lineLen = w + w % 2;
			frame = QVideoFrame(lineLen * h * 2, size, lineLen * 2, fmt);
			frame.map(QAbstractVideoBuffer::ReadWrite);
			size_t offset = 0;
			int r, g, b = 0;
			unsigned char y, u, v;
			QRgb val;
			for (int row = 0; row < h; ++row) {
				for (int col = 0; col < w; ++col) {
					val = img.pixel(col, row);
					r = qRed(val);
					g = qGreen(val);
					b = qBlue(val);
					y = ((r * 66 + g * 129 + b * 25) >> 8) + 16;
					u = ((r * (-38) + g * (-74) + b * 112) >> 8) + 128;
					v = ((r * 112 + g * (-94) + b * (-18)) >> 8) + 128;
					frame.bits()[(offset + col) * 2] = y;
					if (col % 2 == 0) {
						frame.bits()[(offset + col) * 2 + 1] = u;
					}
					else {
						frame.bits()[(offset + col) * 2 + 1] = v;
					}
				}
				offset += lineLen;
			}
		}
		if (!frame.isValid()) {
			return false;
		}
		m_xv->present(frame);
		return false;
	}
	return QObject::eventFilter(obj, event);
}

void TestWindow::changeAdaptor(QAction *adaptor)
{
	m_xv->setAdaptor(adaptor->data().value<XvPortID>());
	updateFormats();
}

void TestWindow::changeFormat(QAction *format)
{
	m_xv->setFormat(format->data().value<int>());
}

void TestWindow::updateFormats()
{
	while (m_formats->actions().count()) {
		QAction *action = m_formats->actions().first();
		m_formatGroup->removeAction(action);
		m_formats->removeAction(action);
		action->deleteLater();
	}

	QtXvWidget::PixelFormatList formats = m_xv->formats();
	foreach (const QtXvWidget::PixelFormatInfo &format, formats) {
		QAction *action = new QAction(format.name(), this);
		action->setCheckable(true);
		if (format.id == m_xv->format()) {
			action->setChecked(true);
		}
		action->setData(QVariant::fromValue<int>(format.id));
		m_formatGroup->addAction(action);
		m_formats->addAction(action);
	}
}

