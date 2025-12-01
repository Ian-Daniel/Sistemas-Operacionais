#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "trem.h"

/* Janela principal da aplicação */:
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0); // Construtor;
    ~MainWindow();                            // Destrutor.

public slots:
    void updateInterface(int, int, int); // Atualiza a interface;
    void init_trem();                    // Inicializa os trens.

private slots:
    // Ajuste de velocidade dos trens:
    void on_horizontalSlider_1_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_3_valueChanged(int value);
    void on_horizontalSlider_4_valueChanged(int value);
    void on_horizontalSlider_5_valueChanged(int value);
    void on_horizontalSlider_6_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    // Ponteiros para os trens:
    Trem *trem1;
    Trem *trem2;
    Trem *trem3;
    Trem *trem4;
    Trem *trem5;
    Trem *trem6;
};

#endif