#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>
#include <QWidget>

class LineEdit : public QWidget
{
    Q_OBJECT
public:
    explicit LineEdit(const QString &labelName, QWidget *parent = nullptr);

private:
    QLabel *label;
    QLineEdit *edit;

signals:
    void valueChanged(int);

private slots:
    void save(QSettings &settings);
    void load(QSettings &settings);

    void textChanged(const QString &text);
};

#endif // LINEEDIT_H
