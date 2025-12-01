#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <semaphore.h>

// Semáforos das regiões críticas (7 internos + 4 externos do trem 6):
sem_t semaforos[11];

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Instancia os trens (ID, X, Y, velocidade inicial):
    trem1 = new Trem(1, 40, 40, ui->horizontalSlider_1->value());
    trem2 = new Trem(2, 310, 40, ui->horizontalSlider_2->value());
    trem3 = new Trem(3, 580, 40, ui->horizontalSlider_3->value());
    trem4 = new Trem(4, 170, 160, ui->horizontalSlider_4->value());
    trem5 = new Trem(5, 440, 160, ui->horizontalSlider_5->value());
    trem6 = new Trem(6, 20, 300, ui->horizontalSlider_6->value()); // Malha externa.

    // Conexão dos sinais de atualização:
    connect(trem1, SIGNAL(updateGUI(int, int, int, int)), SLOT(updateInterface(int, int, int)));
    connect(trem2, SIGNAL(updateGUI(int, int, int, int)), SLOT(updateInterface(int, int, int)));
    connect(trem3, SIGNAL(updateGUI(int, int, int, int)), SLOT(updateInterface(int, int, int)));
    connect(trem4, SIGNAL(updateGUI(int, int, int, int)), SLOT(updateInterface(int, int, int)));
    connect(trem5, SIGNAL(updateGUI(int, int, int, int)), SLOT(updateInterface(int, int, int)));
    connect(trem6, SIGNAL(updateGUI(int, int, int, int)), SLOT(updateInterface(int, int, int)));

    // Inicializa semáforos (todas as regiões começam liberadas):
    for (int i = 0; i < 11; i++)
        sem_init(&semaforos[i], 0, 1);

    init_trem(); // Inicia os trens.
}

void MainWindow::updateInterface(int id, int x, int y)
{
    // Atualiza posição do trem correspondente:
    switch (id)
    {
    case 1:
        ui->label_trem1->setGeometry(x, y, 21, 17);
        break;
    case 2:
        ui->label_trem2->setGeometry(x, y, 21, 17);
        break;
    case 3:
        ui->label_trem3->setGeometry(x, y, 21, 17);
        break;
    case 4:
        ui->label_trem4->setGeometry(x, y, 21, 17);
        break;
    case 5:
        ui->label_trem5->setGeometry(x, y, 21, 17);
        break;
    case 6:
        ui->label_trem6->setGeometry(x, y, 21, 17);
        break;
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Inicia as threads dos trens:
void MainWindow::init_trem()
{
    trem1->start();
    trem2->start();
    trem3->start();
    trem4->start();
    trem5->start();
    trem6->start();
}

// Controle de velocidade dos trens:
void MainWindow::on_horizontalSlider_1_valueChanged(int value) { trem1->setVel(value); }
void MainWindow::on_horizontalSlider_2_valueChanged(int value) { trem2->setVel(value); }
void MainWindow::on_horizontalSlider_3_valueChanged(int value) { trem3->setVel(value); }
void MainWindow::on_horizontalSlider_4_valueChanged(int value) { trem4->setVel(value); }
void MainWindow::on_horizontalSlider_5_valueChanged(int value) { trem5->setVel(value); }
void MainWindow::on_horizontalSlider_6_valueChanged(int value) { trem6->setVel(value); }