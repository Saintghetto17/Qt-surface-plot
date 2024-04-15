#include "gradientpicker.h"

GradientPicker::GradientPicker(const QString &name,
                               const QVector<QLinearGradient> &gradients,
                               QWidget *parent)
    : QGroupBox(name, parent)
{
    group = new QButtonGroup(this);

    QVBoxLayout *vbox = new QVBoxLayout;
    int id = 0;
    for (QLinearGradient g : gradients) {
        QPushButton *button = new QPushButton(this);
        g.setFinalStop(button->width(), 1);
        QPixmap pm(button->size());
        QPainter painter(&pm);

        painter.setBrush(QBrush(g));
        painter.setPen(Qt::NoPen);
        painter.drawRect(QRect({0, 0}, button->size()));

        button->setIcon(QIcon(pm));
        button->setIconSize(button->size());
        group->addButton(button, id++);
        vbox->addWidget(button);
    }
    setLayout(vbox);
    QObject::connect(group, SIGNAL(buttonClicked(int)), this, SIGNAL(gradientPicked(int)));
}
