#include "mainwindow.h"
#include "checkbox.h"
#include "gradientpicker.h"
#include "lineedit.h"
#include "plot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initMenuBar();
    initToolBar();
    initStatusBar();

    CheckBox *showGrid = new CheckBox("Show grid", this);
    CheckBox *showLabel = new CheckBox("Show label", this);
    CheckBox *showLabelBorder = new CheckBox("Show label border", this);
    QObject::connect(showLabel, SIGNAL(stateChanged(int)), showLabelBorder, SLOT(setEnabled(int)));

    auto functions = createFunctions();
    QVector<QString> functionNames;
    for (int i = 0; i < functions.length(); ++i) {
        functionNames.push_back("Sinc" + QString::number(i + 1));
    }

    OptionPicker *plotPicker = new OptionPicker("Plots", functionNames, this);
    OptionPicker *selectionPicker = new OptionPicker("Selection mode",
                                                     {"No selection", "Items"},
                                                     this);

    LineEdit *samplesX = new LineEdit("samplesX", this);
    LineEdit *samplesZ = new LineEdit("samplesZ", this);

    RangePicker *colRange = new RangePicker("Column range", this);
    RangePicker *rowRange = new RangePicker("Row range", this);

    auto gradients = createGradients();
    GradientPicker *gradientPicker = new GradientPicker("Gradient", gradients, this);

    Plot *plot = new Plot(createFunctions(), gradients, this);
    QObject::connect(colRange, SIGNAL(rangeChanged(int, int)), plot, SLOT(setRangeX(int, int)));
    QObject::connect(rowRange, SIGNAL(rangeChanged(int, int)), plot, SLOT(setRangeZ(int, int)));
    QObject::connect(plotPicker, SIGNAL(optionPicked(int)), plot, SLOT(pickGraph(int)));
    QObject::connect(selectionPicker, SIGNAL(optionPicked(int)), plot, SLOT(setSelectionMode(int)));
    QObject::connect(showGrid, SIGNAL(stateChanged(int)), plot, SLOT(showGrid(int)));
    QObject::connect(showLabel, SIGNAL(stateChanged(int)), plot, SLOT(showLabel(int)));
    QObject::connect(showLabelBorder, SIGNAL(stateChanged(int)), plot, SLOT(showLabelBorder(int)));
    QObject::connect(gradientPicker, SIGNAL(gradientPicked(int)), plot, SLOT(setGradient(int)));
    QObject::connect(samplesX, SIGNAL(valueChanged(int)), plot, SLOT(setSamplesX(int)));
    QObject::connect(samplesZ, SIGNAL(valueChanged(int)), plot, SLOT(setSamplesZ(int)));

    QWidget *window = new QWidget();
    setCentralWidget(window);

    QVBoxLayout *vLayout = new QVBoxLayout(window);
    vLayout->addWidget(showGrid);
    vLayout->addWidget(showLabel);
    vLayout->addWidget(showLabelBorder);
    vLayout->addWidget(plotPicker);
    vLayout->addWidget(selectionPicker);
    vLayout->addWidget(samplesX);
    vLayout->addWidget(samplesZ);
    vLayout->addWidget(colRange);
    vLayout->addWidget(rowRange);
    vLayout->addWidget(gradientPicker);
    vLayout->setAlignment(Qt::AlignTop);

    QWidget *settingsWidget = new QWidget();
    settingsWidget->setLayout(vLayout);
    settingsWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    QHBoxLayout *lay = new QHBoxLayout();
    lay->addWidget(plot->getContainer());
    lay->addWidget(settingsWidget);

    window->setLayout(lay);

    loadSettings();
}

MainWindow::~MainWindow() {}

void MainWindow::initMenuBar()
{
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *fileMenu = menuBar->addMenu(tr("&Settings"));
    QAction *action = fileMenu->addAction(tr("&Load"), this, SLOT(loadSettings()));
    action->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    action->setStatusTip("Load");
    action->setShortcut(QKeySequence::Open);

    action = fileMenu->addAction(tr("&Save"), this, SLOT(saveSettings()));
    action->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    action->setStatusTip("Save");
    action->setShortcut(QKeySequence::Save);

    menuBar->addMenu(fileMenu);
    setMenuBar(menuBar);
}

void MainWindow::initToolBar()
{
    QToolBar *toolbar = new QToolBar(this);
    QAction *action = toolbar->addAction("Load", this, SLOT(loadSettings()));
    action->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    action->setStatusTip("Load");
    action = toolbar->addAction("Save", this, SLOT(saveSettings()));
    action->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    action->setStatusTip("Save");
    addToolBar(toolbar);
}

void MainWindow::initStatusBar()
{
    setStatusBar(new QStatusBar(this));
}

QVector<QLinearGradient> MainWindow::createGradients()
{
    QVector<QLinearGradient> gradients;
    gradients.push_back({});
    gradients.back().setStops(
        {{1.f, Qt::red}, {0.8f, Qt::yellow}, {0.5f, Qt::green}, {0.2f, Qt::blue}, {0.f, Qt::black}});
    gradients.push_back({});
    gradients.back().setStops(
        {{1.f, Qt::white}, {0.7f, Qt::yellow}, {0.3f, Qt::red}, {0.f, Qt::black}});
    gradients.push_back({});
    gradients.back().setStops(QGradient(QGradient::NorthMiracle).stops());
    return gradients;
}

QVector<Plot::Function> MainWindow::createFunctions()
{
    QVector<Plot::Function> funcions;
    funcions.push_back([](float x, float z) -> float {
        float R = qSqrt(z * z + x * x) + 0.01f;
        return qSin(R) / R;
    });
    funcions.push_back([](float x, float z) -> float { return qSin(x) / x * qSin(z) / z; });
    funcions.push_back([](float x, float z) -> float { return qSin(x) / x + qCos(z) / 2; });
    funcions.push_back([](float x, float z) -> float {
        x *= 2;
        z *= 2;
        float R = qSqrt(z * z + x * x);
        return R + 3 * qCos(R);
    });
    return funcions;
}

void MainWindow::saveSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    emit save(settings);
}

void MainWindow::loadSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    emit load(settings);
}
