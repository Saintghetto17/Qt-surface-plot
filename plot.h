#ifndef PLOT2_H
#define PLOT2_H

#include <QAbstractItemView>
#include <QScreen>
#include <QWidget>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QSurfaceDataArray>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization>

class Plot : public QObject
{
    Q_OBJECT
public:
    using Function = float (*)(float, float);

    explicit Plot(const QVector<Function> &functions,
                  const QVector<QLinearGradient> &gradients,
                  QWidget *parent);
    QWidget *getContainer();

private:
    QWidget *container;

    QVector<QLinearGradient> gradients;
    QVector<int> gradientId;

    QtDataVisualization::Q3DSurface *surface;

    QVector<QtDataVisualization::QSurface3DSeries *> series;
    QVector<Function> functions;

    QSizeF sampleMinMax;
    QSizeF rangeX;
    QSizeF rangeZ;

    int sampleX;
    int sampleZ;
    int currentGraph;

    void calculateSeries();
    void fillSqrtSinProxy();
    void fillSqrtSqrtSinProxy();
    void initContainer();

signals:

public slots:
    void setRangeX(int min, int max);
    void setRangeZ(int min, int max);
    void pickGraph(int index);
    void setSelectionMode(int index);
    void showGrid(int enable);
    void showLabel(int enable);
    void showLabelBorder(int enable);
    void setGradient(int index);

    void setSamplesX(int value);
    void setSamplesZ(int value);

private slots:
    void save(QSettings &settings);
    void load(QSettings &settings);
};

#endif // PLOT2_H
