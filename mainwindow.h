#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "optionpicker.h"
#include "plot.h"
#include "rangepicker.h"
#include <Q3DSurface>
#include <Q3DTheme>
#include <QAbstract3DGraph>
#include <QCheckBox>
#include <QDebug>
#include <QGradientStops>
#include <QGroupBox>
#include <QKeySequence>
#include <QLabel>
#include <QLineEdit>
#include <QLinearGradient>
#include <QMainWindow>
#include <QMenuBar>
#include <QPushButton>
#include <QRadioButton>
#include <QScreen>
#include <QSettings>
#include <QSlider>
#include <QStatusBar>
#include <QString>
#include <QStyle>
#include <QSurfaceDataProxy>
#include <QToolBar>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initMenuBar();
    void initToolBar();
    void initStatusBar();
    QVector<QLinearGradient> createGradients();
    QVector<Plot::Function> createFunctions();

private slots:
    void saveSettings();
    void loadSettings();

signals:
    void save(QSettings &);
    void load(QSettings &);
};
#endif // MAINWINDOW_H
