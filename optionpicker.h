#ifndef OPTIONICKER_H
#define OPTIONICKER_H

#include <QBoxLayout>
#include <QButtonGroup>
#include <QGroupBox>
#include <QRadioButton>
#include <QSettings>

class OptionPicker : public QGroupBox
{
    Q_OBJECT
public:
    explicit OptionPicker(const QString &name,
                          const QVector<QString> &buttonNames,
                          QWidget *parent = nullptr);

private:
    QButtonGroup *group;

signals:
    void optionPicked(int);

private slots:
    void save(QSettings &settings);
    void load(QSettings &settings);
};

#endif // OPTIONICKER_H
