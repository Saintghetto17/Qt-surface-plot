#include "optionpicker.h"

OptionPicker::OptionPicker(const QString &name, const QVector<QString> &buttonNames, QWidget *parent)
    : QGroupBox(name, parent)
{
    group = new QButtonGroup(this);

    QVBoxLayout *vbox = new QVBoxLayout;
    int id = 0;
    for (const QString &buttonName : buttonNames) {
        QRadioButton *button = new QRadioButton(buttonName, this);
        group->addButton(button, id++);
        vbox->addWidget(button);
    }
    setLayout(vbox);
    QObject::connect(group, SIGNAL(buttonClicked(int)), this, SIGNAL(optionPicked(int)));
    QObject::connect(parent, SIGNAL(load(QSettings &)), this, SLOT(load(QSettings &)));
    QObject::connect(parent, SIGNAL(save(QSettings &)), this, SLOT(save(QSettings &)));
}

void OptionPicker::save(QSettings &settings)
{
    settings.beginGroup(title());
    settings.setValue("id", group->checkedId());
    settings.endGroup();
}

void OptionPicker::load(QSettings &settings)
{
    settings.beginGroup(title());
    int checkedId = settings.value("id", 0).toInt();
    settings.endGroup();
    group->button(checkedId)->setChecked(true);
    emit optionPicked(checkedId);
}
