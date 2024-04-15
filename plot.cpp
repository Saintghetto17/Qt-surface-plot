#include "plot.h"
using namespace QtDataVisualization;

Plot::Plot(const QVector<Function> &functions,
           const QVector<QLinearGradient> &gradients,
           QWidget *parent)
    : gradients(gradients)
    , functions(functions)
{
    QObject::connect(parent, SIGNAL(load(QSettings &)), this, SLOT(load(QSettings &)));
    QObject::connect(parent, SIGNAL(save(QSettings &)), this, SLOT(save(QSettings &)));

    surface = new QtDataVisualization::Q3DSurface;
    initContainer();

    surface->setAxisX(new QValue3DAxis);
    surface->setAxisY(new QValue3DAxis);
    surface->setAxisZ(new QValue3DAxis);

    for (int i = 0; i < functions.length(); ++i) {
        series.push_back(new QSurface3DSeries(new QSurfaceDataProxy));
    }

    sampleMinMax = {-10, 10};
    sampleX = -1;
    sampleZ = -1;
    currentGraph = -1;

    //surface->activeTheme()->setGridLineColor(Qt::black);
}

void Plot::initContainer()
{
    container = QWidget::createWindowContainer(surface);
    QSize screenSize = surface->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);
}

QWidget *Plot::getContainer()
{
    return container;
}

void Plot::setRangeX(int min, int max)
{
    surface->axisX()->setRange(min, max);
}

void Plot::setRangeZ(int min, int max)
{
    surface->axisZ()->setRange(min, max);
}

void Plot::pickGraph(int index)
{
    if (index == currentGraph) {
        return;
    }

    if (currentGraph != -1) {
        surface->removeSeries(series[currentGraph]);
    }

    series[index]->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    surface->addSeries(series[index]);
    currentGraph = index;
}

void Plot::setSelectionMode(int index)
{
    if (index) {
        surface->setSelectionMode(QAbstract3DGraph::SelectionItem);
    } else {
        surface->setSelectionMode(QAbstract3DGraph::SelectionNone);
    }
}

void Plot::showGrid(int enable)
{
    surface->activeTheme()->setGridEnabled(enable);
}

void Plot::showLabel(int enable)
{
    for (auto *axis : surface->axes()) {
        axis->setLabelFormat(enable ? "%1.f" : "");
    }
}

void Plot::showLabelBorder(int enable)
{
    surface->activeTheme()->setLabelBorderEnabled(enable);
}

void Plot::setGradient(int index)
{
    gradientId[currentGraph] = index;
    surface->seriesList().back()->setBaseGradient(gradients[index]);
}

void Plot::setSamplesX(int value)
{
    sampleX = value;
    calculateSeries();
}

void Plot::setSamplesZ(int value)
{
    sampleZ = value;
    calculateSeries();
}

void Plot::save(QSettings &settings)
{
    settings.beginGroup("Plot");
    QList<QVariant> ids;
    for (int id : gradientId) {
        ids.push_back(id);
    }
    settings.setValue("gradientId", ids);
    settings.endGroup();
}

void Plot::load(QSettings &settings)
{
    settings.beginGroup("Plot");
    QVector<int> ids;

    for (const QVariant &id : settings.value("gradientId", QList<QVariant>()).toList()) {
        ids.push_back(id.toInt());
    }

    if (ids.length() != series.length()) {
        gradientId = QVector<int>(series.length(), 0);
    } else {
        gradientId = ids;
    }

    for (int i = 0; i < series.length(); ++i) {
        series[i]->setBaseGradient(gradients[gradientId[i]]);
        series[i]->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    }

    settings.endGroup();
}

void Plot::calculateSeries()
{
    if (sampleX == -1 || sampleZ == -1) {
        return;
    }
    for (int index = 0; index < series.size(); ++index) {
        Function f = functions[index];
        float stepX = (sampleMinMax.height() - sampleMinMax.width()) / float(sampleX - 1);
        float stepZ = (sampleMinMax.height() - sampleMinMax.width()) / float(sampleZ - 1);
        QSurfaceDataArray *dataArray = new QSurfaceDataArray();
        dataArray->reserve(sampleZ);
        for (int i = 0; i < sampleZ; i++) {
            QSurfaceDataRow *newRow = new QSurfaceDataRow(sampleX);
            float z = qMin(sampleMinMax.height(), (i * stepZ + sampleMinMax.width()));
            int index = 0;
            for (int j = 0; j < sampleX; j++) {
                float x = qMin(sampleMinMax.height(), (j * stepX + sampleMinMax.width()));
                (*newRow)[index++].setPosition(QVector3D(x, f(x, z), z));
            }
            *dataArray << newRow;
        }
        series[index]->dataProxy()->resetArray(dataArray);
    }
}
