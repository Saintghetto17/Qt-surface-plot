#include "mainwindow.h"

#include <Q3DSurface>
#include <QApplication>
#include <QHBoxLayout>
#include <QHeightMapSurfaceDataProxy>
#include <QSurface3DSeries>
#include <QSurfaceDataProxy>
#include <QVBoxLayout>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
