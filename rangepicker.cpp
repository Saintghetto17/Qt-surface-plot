#include "rangepicker.h"

RangePicker::RangePicker(const QString &name, QWidget *parent)
    : QGroupBox(name, parent)
{
    minSlider = new QSlider(Qt::Horizontal, this);
    maxSlider = new QSlider(Qt::Horizontal, this);

    minSlider->setMinimum(-10);
    minSlider->setMaximum(10);
    maxSlider->setMinimum(-10);
    maxSlider->setMaximum(10);

    minEdit = new QLineEdit(this);
    minEdit->setFixedWidth(minSlider->width() / 2);
    maxEdit = new QLineEdit(this);
    maxEdit->setFixedWidth(minSlider->width() / 2);

    QVBoxLayout *vbox = new QVBoxLayout;

    QHBoxLayout *editLayout = new QHBoxLayout;
    editLayout->addWidget(minEdit);
    editLayout->addWidget(maxEdit);

    vbox->addWidget(minSlider);
    vbox->addWidget(maxSlider);
    vbox->addLayout(editLayout);

    setLayout(vbox);

    QObject::connect(minSlider, SIGNAL(valueChanged(int)), this, SLOT(minValueChanged(int)));
    QObject::connect(maxSlider, SIGNAL(valueChanged(int)), this, SLOT(maxValueChanged(int)));

    QObject::connect(minEdit,
                     SIGNAL(textEdited(const QString &)),
                     this,
                     SLOT(minEdited(const QString &)));
    QObject::connect(maxEdit,
                     SIGNAL(textEdited(const QString &)),
                     this,
                     SLOT(maxEdited(const QString &)));

    QObject::connect(parent, SIGNAL(load(QSettings &)), this, SLOT(load(QSettings &)));
    QObject::connect(parent, SIGNAL(save(QSettings &)), this, SLOT(save(QSettings &)));
}

void RangePicker::save(QSettings &settings)
{
    settings.beginGroup(title());
    settings.setValue("range", QSize(minSlider->minimum(), minSlider->maximum()));
    settings.setValue("values", QSize(minSlider->value(), maxSlider->value()));
    settings.endGroup();
}

void RangePicker::load(QSettings &settings)
{
    settings.beginGroup(title());
    QSize range = settings.value("range", QSize(-10, 10)).toSize();
    QSize value = settings.value("values", range).toSize();
    settings.endGroup();

    minSlider->setValue(value.width());
    maxSlider->setValue(value.height());
}

void RangePicker::minValueChanged(int value)
{
    if (minSlider->maximum() == value) {
        minSlider->setValue(value - 1);
        return;
    }

    minEdit->setText(QString::number(value));
    if (maxSlider->value() <= value + 1) {
        maxSlider->setValue(value + 1);
    }

    emit rangeChanged(minSlider->value(), maxSlider->value());
}

void RangePicker::maxValueChanged(int value)
{
    if (maxSlider->minimum() == value) {
        maxSlider->setValue(value + 1);
        return;
    }

    maxEdit->setText(QString::number(value));
    if (minSlider->value() >= value - 1) {
        minSlider->setValue(value - 1);
    }
    emit rangeChanged(minSlider->value(), maxSlider->value());
}

void RangePicker::minEdited(const QString &str)
{
    minSlider->setValue(str.toInt());
}

void RangePicker::maxEdited(const QString &str)
{
    maxSlider->setValue(str.toInt());
}
