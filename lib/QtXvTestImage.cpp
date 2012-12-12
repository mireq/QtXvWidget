#include <QColor>
#include <QPainter>
#include <QPoint>
#include "QtXvTestImage.h"

QImage QtXvTestImage::generateImage(int width, int height)
{
	QVector<Qt::GlobalColor> colors;
	colors << Qt::white;
	colors << Qt::black;
	colors << Qt::red;
	colors << Qt::green;
	colors << Qt::blue;

	QImage img(width, height, QImage::Format_ARGB32);
	QPainter painter(&img);

	for (int i = 0; i < colors.count(); ++i) {
		QColor color = colors[i];
		QRect bar(QPoint(0, height * i / colors.count()), QPoint(width, height * (i + 1) / colors.count()));
		painter.fillRect(bar, color);
		int border = height / (colors.count() * 4);
		bar.adjust(border, border, -border, -border);
		int x = bar.x();
		int y = bar.y();
		int w = bar.width();
		int h = bar.height();
		for (int j = 0; j < 15; ++j) {
			QRect sub(QPoint(x + w * j / 15, y), QPoint(x + w * (j + 1) / 15, y + h));
			if (colors[i] == Qt::white) {
				color = QColor(255 - j * 4 - 4, 255 - j * 4 - 4, 255 - j * 4 - 4);
				painter.fillRect(sub, color);
			}
			else if (colors[i] == Qt::black) {
				color = QColor(j * 4 + 4, j * 4 + 4, j * 4 + 4);
				painter.fillRect(sub, color);
			}
			else {
				color = QColor(colors[i]).lighter(100 + (j - 7) * 2);
				painter.fillRect(sub, color);
			}
		}
	}
	painter.end();
	return img;
}

