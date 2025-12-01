#ifndef TREM_H
#define TREM_H

#include <QThread>
#include <QVector>
#include <QPoint>
#include <semaphore.h>

class Trem : public QThread {
    Q_OBJECT
public:
    Trem(int ID, int x, int y);
    ~Trem();
    void run();
    void setVelocidade(int vel);
    void stop();

signals:
    void updateGUI(int ID, int x, int y);

private:
    int x;
    int y;
    int ID;
    int velocidade;
    bool parado;
    
    // Caminho do trem:
    QVector<QPoint> caminho;
    int indiceAtual;

    // Regiões críticas alocadas:
    QVector<int> regioesAlocadas;

    // Métodos auxiliares:
    void moverPara(int targetX, int targetY);
    int obterRegiaoCritica(int indiceDe, int indicePara);
    int calcularSleep();
};

#endif // TREM_H