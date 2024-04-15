#include "checkbox.h"

CheckBox::CheckBox(const QString &name, QWidget *parent)
    : QCheckBox(name, parent)
{
    QObject::connect(parent, SIGNAL(load(QSettings &)), this, SLOT(load(QSettings &)));
    QObject::connect(parent, SIGNAL(save(QSettings &)), this, SLOT(save(QSettings &)));
}

void CheckBox::setEnabled(int enabled)
{
    QCheckBox::setEnabled(enabled);
}

void CheckBox::save(QSettings &settings)
{
    settings.beginGroup(text());
    settings.setValue("checked", isChecked());
    settings.endGroup();
}

void CheckBox::load(QSettings &settings)
{
    settings.beginGroup(text());
    setChecked(settings.value("checked", false).toBool());
    settings.endGroup();

    emit stateChanged(isChecked());
}
