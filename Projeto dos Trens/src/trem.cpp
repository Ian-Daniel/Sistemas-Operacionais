#include "trem.h"
#include <QtCore>
#include <cmath>
#include <algorithm>

// Semáforos globais:
extern sem_t semaforos[7];

Trem::Trem(int ID, int x, int y) {
    this->ID = ID;
    this->x = x;
    this->y = y;
    this->velocidade = 100;
    this->parado = false;
    this->indiceAtual = 0;

    // Pontos da malha:
    QPoint pTipEsq(120, 200);
    QPoint pTopEsq(260, 60);
    QPoint pTopDir(540, 60);
    QPoint pTipDir(680, 200);
    QPoint pBotDir(540, 340);
    QPoint pBotEsq(260, 340);
    QPoint pMidEsq(260, 200);
    QPoint pMidDir(540, 200);

    // Definição dos caminhos:
    switch(ID) {
        case 1: // Trem verde.
            caminho << pTipEsq << pMidEsq << pTopEsq;
            this->x = pTipEsq.x(); this->y = pTipEsq.y();
            break;
        case 2: // Trem vermelho.
            caminho << pTopEsq << pMidEsq << pMidDir << pTopDir;
            this->x = pTopEsq.x(); this->y = pTopEsq.y();
            break;
        case 3: // Trem azul.
            caminho << pTopDir << pMidDir << pTipDir;
            this->x = pTopDir.x(); this->y = pTopDir.y();
            break;
        case 4: // Trem laranja.
            caminho << pTipEsq << pBotEsq << pMidEsq;
            this->x = pTipEsq.x(); this->y = pTipEsq.y();
            break;
        case 5: // Trem amarelo.
            caminho << pMidEsq << pBotEsq << pBotDir << pMidDir;
            this->x = pMidEsq.x(); this->y = pMidEsq.y();
            break;
        case 6: // Trem roxo.
            caminho << pMidDir << pBotDir << pTipDir;
            this->x = pBotDir.x(); this->y = pBotDir.y();
            break;
    }
}

Trem::~Trem() {
    stop();
    wait();
}

void Trem::setVelocidade(int vel) {
    this->velocidade = vel;
}

void Trem::stop() {
    parado = true;
}

int Trem::obterRegiaoCritica(int i1, int i2) {
    QPoint p1 = caminho[i1];
    QPoint p2 = caminho[i2];

    // Região 0.
    if ((p1.x() == 260 && p2.x() == 260) && ((p1.y() == 60 && p2.y() == 200) || (p1.y() == 200 && p2.y() == 60))) return 0;
    // Região 1.
    if ((p1.x() == 540 && p2.x() == 540) && ((p1.y() == 60 && p2.y() == 200) || (p1.y() == 200 && p2.y() == 60))) return 1;
    // Região 2.
    if ((p1.y() == 200 && p2.y() == 200) && ((p1.x() == 120 && p2.x() == 260) || (p1.x() == 260 && p2.x() == 120))) return 2;
    // Região 3.
    if ((p1.y() == 200 && p2.y() == 200) && ((p1.x() == 260 && p2.x() == 540) || (p1.x() == 540 && p2.x() == 260))) return 3;
    // Região 4.
    if ((p1.y() == 200 && p2.y() == 200) && ((p1.x() == 540 && p2.x() == 680) || (p1.x() == 680 && p2.x() == 540))) return 4;
    // Região 5.
    if ((p1.x() == 260 && p2.x() == 260) && ((p1.y() == 200 && p2.y() == 340) || (p1.y() == 340 && p2.y() == 200))) return 5;
    // Região 6.
    if ((p1.x() == 540 && p2.x() == 540) && ((p1.y() == 200 && p2.y() == 340) || (p1.y() == 340 && p2.y() == 200))) return 6;

    return -1; // Livre.
}

int Trem::calcularSleep() {
    if (velocidade == 0) return 100;
    int sleep = 200 - velocidade;
    return (sleep < 10) ? 10 : sleep;
}

void Trem::run() {
    if (caminho.isEmpty()) return;

    // Reserva inicial:
    int proximo = (indiceAtual + 1) % caminho.size();
    int regiaoInicial = obterRegiaoCritica(indiceAtual, proximo);
    if (regiaoInicial != -1) {
        sem_wait(&semaforos[regiaoInicial]);
        regioesAlocadas.append(regiaoInicial);
    }

    const int DISTANCIA_FRENAGEM = 60;

    while (!parado) {
        while (velocidade == 0 && !parado) { msleep(100); }
        if (parado) break;

        int proximoIndice = (indiceAtual + 1) % caminho.size();
        QPoint alvo = caminho[proximoIndice];

        // Look-ahead de regiões:
        int indiceFuturo = (proximoIndice + 1) % caminho.size();
        int regiaoAtual = obterRegiaoCritica(indiceAtual, proximoIndice);
        int regiaoFutura = obterRegiaoCritica(proximoIndice, indiceFuturo);

        bool podeMover = true;

        while ((x != alvo.x() || y != alvo.y()) && !parado) {
            
            int dx = alvo.x() - x;
            int dy = alvo.y() - y;
            double distancia = std::sqrt(dx*dx + dy*dy);
            // Reserva dupla para regiões críticas específicas.

            if (distancia <= DISTANCIA_FRENAGEM) {
                if (regiaoFutura != -1 && !regioesAlocadas.contains(regiaoFutura)) {
                    
                    bool precisaCombo = false;
                    // T2 → região 3 exige 1.
                    if (ID == 2 && regiaoFutura == 3) precisaCombo = true;
                    // T3 → região 1 exige 4.
                    if (ID == 3 && regiaoFutura == 1) precisaCombo = true;
                    // T5 → região 6 exige 3.
                    if (ID == 5 && regiaoFutura == 6) precisaCombo = true;
                    // T6 → região 4 exige 6.
                    if (ID == 6 && regiaoFutura == 4) precisaCombo = true;

                    if (precisaCombo) {
               
                        int indiceAposFuturo = (indiceFuturo + 1) % caminho.size();
                        int regiaoDepoisDaFutura = obterRegiaoCritica(indiceFuturo, indiceAposFuturo);
                        
                        // Tentativa de reserva dupla:
                        if (sem_trywait(&semaforos[regiaoFutura]) == 0) {
                            if (sem_trywait(&semaforos[regiaoDepoisDaFutura]) == 0) {
                                regioesAlocadas.append(regiaoFutura);
                                regioesAlocadas.append(regiaoDepoisDaFutura);
                            } else {
                                sem_post(&semaforos[regiaoFutura]);
                                podeMover = false;
                            }
                        } else {
                            podeMover = false;
                        }
                    } else {
                        // Reserva simples:
                        if (sem_trywait(&semaforos[regiaoFutura]) == 0) {
                            regioesAlocadas.append(regiaoFutura);
                        } else {
                            podeMover = false;
                        }
                    }
                }
            }

            if (!podeMover) {
                msleep(10); // Reduz velocidade na espera.
                podeMover = true;
                continue; 
            }

            moverPara(alvo.x(), alvo.y());
            emit updateGUI(ID, x, y);
            msleep(calcularSleep());
        }

        // Liberação da região anterior:
        if (regiaoAtual != -1) {
            sem_post(&semaforos[regiaoAtual]);
            regioesAlocadas.removeAll(regiaoAtual);
        }

        indiceAtual = proximoIndice;
    }

    for(int r : regioesAlocadas) {
        sem_post(&semaforos[r]);
    }
}

void Trem::moverPara(int targetX, int targetY) {
    int step = 2;
    if (x < targetX) x += std::min(step, targetX - x);
    else if (x > targetX) x -= std::min(step, x - targetX);
    
    if (y < targetY) y += std::min(step, targetY - y);
    else if (y > targetY) y -= std::min(step, targetY - y);
}