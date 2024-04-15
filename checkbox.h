#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <QCheckBox>
#include <QSettings>

class CheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit CheckBox(const QString &name, QWidget *parent = nullptr);

signals:

public slots:
    void setEnabled(int enabled);
private slots:
    void save(QSettings &settings);
    void load(QSettings &settings);
};

#endif // CHECKBOX_H
