#include <QGridLayout>
#include <QLabel>
#include <QSignalMapper>
#include <QSlider>
#include <QSpinBox>
#include "QtXvControlsWidget.h"


QtXvControlsWidget::QtXvControlsWidget(QtXvWidget *parent):
	QWidget(parent),
	m_xv(parent)
{
	m_layout = new QGridLayout();
	m_mapper = new QSignalMapper(this);
	setLayout(m_layout);
	connect(m_xv, SIGNAL(portChanged(XvPortID)), SLOT(updateControls()));
	connect(m_mapper, SIGNAL(mapped(const QString &)), SLOT(valueChanged(const QString &)));
}

QtXvControlsWidget::~QtXvControlsWidget()
{
}

void QtXvControlsWidget::updateControls()
{
	while (m_layout->count()) {
		QLayoutItem *item = m_layout->takeAt(0);
		if (item->widget()) {
			delete item->widget();
		}
		if (item->layout()) {
			delete item->layout();
		}
		delete item;
	}

	int row = 0;
	QtXvWidget::AttributeList attributes = m_xv->xvAttributes();
	foreach (const QtXvWidget::AttributeInfo &attribute, attributes) {
		if (!(attribute.flags & QtXvWidget::ReadFlag) || !(attribute.flags & QtXvWidget::WriteFlag)) {
			continue;
		}
		QWidget *widget = new QWidget();

		QSpinBox *spinBox = new QSpinBox(widget);
		QSlider *slider = new QSlider(widget);

		spinBox->setRange(attribute.min, attribute.max);
		slider->setRange(attribute.min, attribute.max);
		slider->setOrientation(Qt::Horizontal);

		connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
		connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
		spinBox->setValue(m_xv->getXvAttribute(attribute.name));
		spinBox->setProperty("xvAttribute", QVariant::fromValue<QString>(attribute.name));
		connect(spinBox, SIGNAL(valueChanged(int)), m_mapper, SLOT(map()));
		m_mapper->setMapping(spinBox, attribute.name);

		m_layout->addWidget(new QLabel(attribute.name), row, 0);
		m_layout->addWidget(spinBox, row, 1);
		m_layout->addWidget(slider, row, 2);
		row++;
	}
}

void QtXvControlsWidget::valueChanged(const QString &attribute)
{
	for (int i = 0; i < m_layout->count(); ++i) {
		QLayoutItem *item = m_layout->itemAt(i);
		if (item->widget() && item->widget()->property("xvAttribute") == attribute) {
			m_xv->setXvAttribute(attribute, item->widget()->property("value").value<int>());
		}
	}
}

