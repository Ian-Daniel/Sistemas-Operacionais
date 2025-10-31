#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Trem::regiao1 = &regiao1;
    Trem::regiao2 = &regiao2;
    Trem::regiao3 = &regiao3;
    Trem::regiao4 = &regiao4;
    Trem::regiao5 = &regiao5;
    Trem::regiao6 = &regiao6;
    Trem::regiao7 = &regiao7;

    trem1 = new Trem(1, 60, 80);
    trem2 = new Trem(2, 270, 30);
    trem3 = new Trem(3, 480, 80);
    trem4 = new Trem(4, 130, 210);
    trem5 = new Trem(5, 410, 210);
    trem6 = new Trem(6, 220, 280);

    connect(trem1, SIGNAL(updateGUI(int, int, int)), SLOT(updateInterface(int, int, int)));
    connect(trem2, SIGNAL(updateGUI(int, int, int)), SLOT(updateInterface(int, int, int)));
    connect(trem3, SIGNAL(updateGUI(int, int, int)), SLOT(updateInterface(int, int, int)));
    connect(trem4, SIGNAL(updateGUI(int, int, int)), SLOT(updateInterface(int, int, int)));
    connect(trem5, SIGNAL(updateGUI(int, int, int)), SLOT(updateInterface(int, int, int)));
    connect(trem6, SIGNAL(updateGUI(int, int, int)), SLOT(updateInterface(int, int, int)));

    ui->horizontalSlider->setRange(0, 200);
    ui->horizontalSlider_2->setRange(0, 200);
    ui->horizontalSlider_3->setRange(0, 200);
    ui->horizontalSlider_4->setRange(0, 200);
    ui->horizontalSlider_5->setRange(0, 200);
    ui->horizontalSlider_6->setRange(0, 200);

    ui->horizontalSlider->setValue(100);
    ui->horizontalSlider_2->setValue(100);
    ui->horizontalSlider_3->setValue(100);
    ui->horizontalSlider_4->setValue(100);
    ui->horizontalSlider_5->setValue(100);
    ui->horizontalSlider_6->setValue(100);

    trem1->start();
    trem2->start();
    trem3->start();
    trem4->start();
    trem5->start();
    trem6->start();
}

void MainWindow::updateInterface(int id, int x, int y)
{
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
    trem1->terminate();
    trem2->terminate();
    trem3->terminate();
    trem4->terminate();
    trem5->terminate();
    trem6->terminate();

    trem1->wait();
    trem2->wait();
    trem3->wait();
    trem4->wait();
    trem5->wait();
    trem6->wait();

    delete ui;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    trem1->setVelocidade(value);
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    trem2->setVelocidade(value);
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    trem3->setVelocidade(value);
}

void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    trem4->setVelocidade(value);
}

void MainWindow::on_horizontalSlider_5_valueChanged(int value)
{
    trem5->setVelocidade(value);
}

void MainWindow::on_horizontalSlider_6_valueChanged(int value)
{
    trem6->setVelocidade(value);
}