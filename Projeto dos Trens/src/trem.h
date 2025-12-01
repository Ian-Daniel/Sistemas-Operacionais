#ifndef TREM_H
#define TREM_H

#include <QThread>

// Thread que representa um trem:
class Trem : public QThread
{
    Q_OBJECT

public:
    Trem(int ID, int x, int y, int vel); // Construtor;
    void run() override;                 // Loop da thread.

    void setVel(int value);
    void setX(int x);
    void setY(int y);
    int getX();
    int getY();

signals:
    void updateGUI(int ID, int x, int y, int vel); // Atualização da interface.

private:
    int x;   // Posição X;
    int y;   // Posição Y;
    int ID;  // Identificador;
    int vel; // Velocidade.
};

#endif