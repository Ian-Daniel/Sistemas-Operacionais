#ifndef TREM_H
#define TREM_H

#include <QThread>
#include <QMutex>
#include <vector>

/*
 * Classe Trem herda QThread;
 * Cada trem é uma thread que executa sua própria função RUN;
 * Cada trem possui ID, posição (x,y) e velocidade;
 * As regiões críticas são controladas por mutexes estáticos compartilhados.
 */

class Trem : public QThread
{
    Q_OBJECT
public:
    Trem(int, int, int);
    void run() override;
    void setVelocidade(int);

    // Mutex global que serializa aquisições para evitar deadlocks:
    static QMutex arbiter;

    // Mutexes estáticos das 7 regiões críticas:
    static QMutex *regiao1;
    static QMutex *regiao2;
    static QMutex *regiao3;
    static QMutex *regiao4;
    static QMutex *regiao5;
    static QMutex *regiao6;
    static QMutex *regiao7;

    // Métodos auxiliares para travar/destravar múltiplas regiões de forma ordenada:
    static void lockRegions(const std::vector<int> &regs);
    static void unlockRegions(const std::vector<int> &regs);

signals:
    void updateGUI(int, int, int);

private:
    int x;          // posição X do trem na tela;
    int y;          // posição Y do trem na tela;
    int ID;         // identificador do trem;
    int velocidade; // tempo (ms) de espera entre movimentos.

    void right();
    void left();
    void up();
    void down();
};

#endif // TREM_H