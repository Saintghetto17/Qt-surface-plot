#include "lineedit.h"

LineEdit::LineEdit(const QString &name, QWidget *parent)
    : QWidget(parent)
{
    QObject::connect(parent, SIGNAL(load(QSettings &)), this, SLOT(load(QSettings &)));
    QObject::connect(parent, SIGNAL(save(QSettings &)), this, SLOT(save(QSettings &)));

    label = new QLabel(name, this);
    edit = new QLineEdit(this);
    edit->setFixedWidth(label->width() / 2);

    edit->setValidator(new QIntValidator(2, 200, this));

    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->addWidget(label);
    hbox->addWidget(edit);
    setLayout(hbox);

    QObject::connect(edit,
                     SIGNAL(textChanged(const QString &)),
                     this,
                     SLOT(textChanged(const QString &)));
}

void LineEdit::save(QSettings &settings)
{
    settings.beginGroup(label->text());
    settings.setValue("number", edit->text().toInt());
    settings.endGroup();
}

void LineEdit::load(QSettings &settings)
{
    settings.beginGroup(label->text());
    int value = settings.value("number", 50).toInt();
    edit->setText(QString::number(value));
    settings.endGroup();
}

void LineEdit::textChanged(const QString &text)
{
    emit valueChanged(text.toInt());
}
