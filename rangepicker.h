#ifndef RANGEPICKER_H
#define RANGEPICKER_H

#include <QBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QSettings>
#include <QSlider>

class RangePicker : public QGroupBox
{
    Q_OBJECT
public:
    explicit RangePicker(const QString &name, QWidget *parent = nullptr);

private:
    QSlider *minSlider;
    QSlider *maxSlider;

    QLineEdit *minEdit;
    QLineEdit *maxEdit;

signals:
    void rangeChanged(int, int);

private slots:
    void save(QSettings &settings);
    void load(QSettings &settings);

private slots:
    void minValueChanged(int value);
    void maxValueChanged(int value);

    void minEdited(const QString &str);
    void maxEdited(const QString &str);
};

#endif // RANGEPICKER_H
