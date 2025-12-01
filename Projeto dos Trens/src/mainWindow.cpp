#include "mainwindow.h"
#include "railwaywidget.h"
#include "train.h"
#include "railwaynetwork.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QTimer>
#include <QGroupBox>
#include <QSizePolicy>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_network(new RailwayNetwork())
{
    setupUI();
    setupTrains();

    // Atualiza posições e cores dos trens em tempo real.
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, [this]() {
        QVector<QPoint> positions;
        QVector<QColor> colors;

        for (Train* train : m_trains) {
            positions.append(train->getPosition());
            colors.append(train->getColor());
        }

        m_railwayWidget->setTrainPositions(positions);
        m_railwayWidget->setTrainColors(colors);
    });

    m_updateTimer->start(16);
}

MainWindow::~MainWindow() {
    // Finaliza e libera cada trem antes de encerrar.
    for (Train* train : m_trains) {
        train->stopTrain();
        train->wait();
        delete train;
    }
    delete m_network;
}

void MainWindow::setupUI() {
    setWindowTitle("Simulação de Trens");
    setMinimumSize(1000, 500);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Layout principal: trilhos à esquerda, controles à direita.
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // Área de visualização dos trilhos.
    m_railwayWidget = new RailwayWidget(this);
    m_railwayWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(m_railwayWidget);

    // Painel lateral de controle de velocidade.
    QGroupBox* controlsGroup = new QGroupBox("Controle de Velocidade", this);
    controlsGroup->setFixedWidth(250);

    QVBoxLayout* controlsLayout = new QVBoxLayout(controlsGroup);

    // Seis controles (um por trem).
    for (int i = 0; i < 6; ++i) {
        QWidget* trainControl = new QWidget(this);
        QVBoxLayout* trainLayout = new QVBoxLayout(trainControl);
        trainLayout->setContentsMargins(0, 5, 0, 5);

        QString colorName;
        switch (i) {
            case 0: colorName = "Verde";    break;
            case 1: colorName = "Vermelho"; break;
            case 2: colorName = "Azul";     break;
            case 3: colorName = "Laranja";  break;
            case 4: colorName = "Amarelo";  break;
            case 5: colorName = "Roxo";     break;
        }

        QLabel* label = new QLabel(QString("Trem %1 (%2)").arg(i).arg(colorName), this);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);

        QSlider* slider = new QSlider(Qt::Horizontal, this);
        slider->setMinimum(0);
        slider->setMaximum(200);
        slider->setValue(100);

        m_speedSliders.append(slider);
        m_speedLabels.append(label);

        connect(slider, &QSlider::valueChanged, this, [this, i](int value) {
            if (i < m_trains.size()) {
                m_trains[i]->setSpeed(value);
            }
        });

        trainLayout->addWidget(label);
        trainLayout->addWidget(slider);
        controlsLayout->addWidget(trainControl);
    }

    controlsLayout->addStretch();
    mainLayout->addWidget(controlsGroup);
}

void MainWindow::setupTrains() {
    // Cria e inicia os trens.
    for (int i = 0; i < 6; ++i) {
        Train* train = new Train(i, m_network, this);
        connect(train, &Train::positionChanged, this, &MainWindow::onTrainPositionChanged);
        m_trains.append(train);
        train->start();
    }
}

void MainWindow::onTrainSpeedChanged(int trainId, int speed) {
    if (trainId >= 0 && trainId < m_trains.size()) {
        m_trains[trainId]->setSpeed(speed);
    }
}

void MainWindow::onTrainPositionChanged(int trainId, int x, int y) {
    Q_UNUSED(trainId);
    Q_UNUSED(x);
    Q_UNUSED(y);
}