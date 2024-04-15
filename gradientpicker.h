#ifndef GRADIENTPICKER_H
#define GRADIENTPICKER_H

#include <QBoxLayout>
#include <QButtonGroup>
#include <QGroupBox>
#include <QPainter>
#include <QPushButton>
#include <QSettings>

class GradientPicker : public QGroupBox
{
    Q_OBJECT
public:
    explicit GradientPicker(const QString &name,
                            const QVector<QLinearGradient> &gradients,
                            QWidget *parent = nullptr);

private:
    QButtonGroup *group;

signals:
    void gradientPicked(int);
};

#endif // GRADIENTPICKER_H
