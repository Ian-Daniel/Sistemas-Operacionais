/* // trem.cpp
#include "trem.h"
#include <algorithm>
#include <vector>
#include <QThread>

// =======================
//  DEFINIÇÕES ESTÁTICAS
// =======================

// Mutex global para controle de travamento de múltiplas regiões:
QMutex Trem::arbiter;

// Cada região representa um trecho do trilho compartilhado entre trens:
QMutex *Trem::regiao1 = nullptr;
QMutex *Trem::regiao2 = nullptr;
QMutex *Trem::regiao3 = nullptr;
QMutex *Trem::regiao4 = nullptr;
QMutex *Trem::regiao5 = nullptr;
QMutex *Trem::regiao6 = nullptr;
QMutex *Trem::regiao7 = nullptr;

// ============================
//  MAPEAR ID → MUTEX REGIÃO
// ============================
static inline QMutex *regionPtrById(int id)
{
    switch (id)
    {
    case 1:
        return Trem::regiao1;
    case 2:
        return Trem::regiao2;
    case 3:
        return Trem::regiao3;
    case 4:
        return Trem::regiao4;
    case 5:
        return Trem::regiao5;
    case 6:
        return Trem::regiao6;
    case 7:
        return Trem::regiao7;
    default:
        return nullptr;
    }
}

// ==============================
//  FUNÇÃO PARA TRAVAR REGIÕES
// ==============================
void Trem::lockRegions(const std::vector<int> &regs)
{
    if (regs.empty())
        return;

    // Caso o trem precise apenas de uma região, trava direto (melhor desempenho):
    if (regs.size() == 1)
    {
        QMutex *m = regionPtrById(regs[0]);
        if (m)
            m->lock();
        return;
    }

    // Para múltiplas regiões, ordena IDs e usa arbiter para evitar deadlocks:
    std::vector<int> ordered = regs;
    std::sort(ordered.begin(), ordered.end());

    Trem::arbiter.lock(); // garante exclusividade durante travamentos múltiplos.
    for (int id : ordered)
    {
        QMutex *m = regionPtrById(id);
        if (m)
            m->lock();
    }
    Trem::arbiter.unlock();
}

// ==================================
//  FUNÇÃO PARA LIBERAR REGIÕES
// ==================================
void Trem::unlockRegions(const std::vector<int> &regs)
{
    if (regs.empty())
        return;

    // Desbloqueia em ordem reversa (boa prática de consistência):
    std::vector<int> ordered = regs;
    std::sort(ordered.begin(), ordered.end(), std::greater<int>());
    for (int id : ordered)
    {
        QMutex *m = regionPtrById(id);
        if (m)
            m->unlock();
    }

    // Pequena pausa para suavizar o comportamento na interface:
    QThread::msleep(1);
}

// ==================================
//  CONSTRUTOR E MÉTODOS AUXILIARES
// ==================================
Trem::Trem(int id, int startX, int startY)
{
    ID = id;
    x = startX;
    y = startY;
    velocidade = 50; // velocidade padrão.
}

// Ajusta velocidade (valor 0 "para" o trem):
void Trem::setVelocidade(int velocidade)
{
    if (velocidade == 0)
        this->velocidade = 1000000; // pausa longa (trem parado).
    else
        this->velocidade = 200 - velocidade; // quanto maior o slider, mais rápido.
}

// Movimentos básicos:
void Trem::right() { x += 10; }
void Trem::left() { x -= 10; }
void Trem::up() { y -= 10; }
void Trem::down() { y += 10; }

// ==================================
//  LOOP PRINCIPAL DA THREAD DO TREM
// ==================================
void Trem::run()
{
    // Flags para indicar se o trem está dentro de uma região:
    bool inReg1 = false, inReg2 = false, inReg3 = false, inReg4 = false;
    bool inReg5 = false, inReg6 = false, inReg7 = false;

    while (true)
    {
        switch (ID)
        {
        // =========================
        //  TREM 1
        // =========================
        case 1:
            if (y == 30 && x < 200)
            {
                // Entrando na região 1:
                if (x == 180 && !inReg1)
                {
                    lockRegions({1});
                    inReg1 = true;
                }
                right();
            }
            else if (x == 200 && y < 150)
            {
                // Entrando na região 3:
                if (y == 130 && !inReg3)
                {
                    lockRegions({3});
                    inReg3 = true;
                }
                down();
            }
            else if (x > 60 && y == 150)
            {
                // Saindo das regiões:
                if (x == 180 && inReg1)
                {
                    unlockRegions({1});
                    inReg1 = false;
                }
                if (x == 110 && inReg3)
                {
                    unlockRegions({3});
                    inReg3 = false;
                }
                left();
            }
            else
            {
                up();
            }
            break;

        // =========================
        //  TREM 2
        // =========================
        case 2:
            if (y == 30 && x < 340)
            {
                // Trecho inicial, pode cruzar regiões 1 e 2:
                if (!inReg1 && !inReg2 && x >= 200 && x <= 320)
                {
                    lockRegions({1, 2});
                    inReg1 = inReg2 = true;
                }
                else if (x == 220 && inReg1)
                {
                    unlockRegions({1});
                    inReg1 = false;
                }
                right();
            }
            else if (x == 340 && y < 150)
            {
                // Descendo, passando pelas regiões 2 e 5:
                if (!inReg2 && !inReg5)
                {
                    lockRegions({2, 5});
                    inReg2 = inReg5 = true;
                }
                else if (!inReg5 && inReg2)
                {
                    lockRegions({5});
                    inReg5 = true;
                }
                down();
            }
            else if (x > 200 && y == 150)
            {
                // Cruzando pela base:
                if (!inReg4 && x == 290)
                {
                    lockRegions({4});
                    inReg4 = true;
                }
                if (x == 320 && inReg2)
                {
                    unlockRegions({2});
                    inReg2 = false;
                }
                if (x == 250 && inReg5)
                {
                    unlockRegions({5});
                    inReg5 = false;
                }
                if (x == 220 && !inReg1)
                {
                    lockRegions({1});
                    inReg1 = true;
                }
                left();
            }
            else
            {
                // Subindo novamente:
                if (x == 200 && y == 130 && inReg4)
                {
                    unlockRegions({4});
                    inReg4 = false;
                }
                up();
            }

            // Segurança extra: trava todas regiões se dentro do grande cruzamento:
            if (!inReg1 && !inReg2 && !inReg4 && !inReg5 && x >= 200 && x <= 320 && y >= 30 && y <= 150)
            {
                lockRegions({1, 2, 4, 5});
                inReg1 = inReg2 = inReg4 = inReg5 = true;
            }
            break;

        // =========================
        //  TREM 3
        // =========================
        case 3:
            if (y == 30 && x < 480)
            {
                if (x == 360 && inReg2)
                {
                    unlockRegions({2});
                    inReg2 = false;
                }
                right();
            }
            else if (x == 480 && y < 150)
            {
                down();
            }
            else if (x > 340 && y == 150)
            {
                // Passando por regiões 2 e 6:
                if (!inReg2 && !inReg6)
                {
                    lockRegions({2, 6});
                    inReg2 = inReg6 = true;
                }
                else
                {
                    if (x == 430 && !inReg6)
                    {
                        lockRegions({6});
                        inReg6 = true;
                    }
                    if (x == 360 && !inReg2)
                    {
                        lockRegions({2});
                        inReg2 = true;
                    }
                }
                left();
            }
            else
            {
                if (x == 340 && y == 130 && inReg6)
                {
                    unlockRegions({6});
                    inReg6 = false;
                }
                up();
            }
            break;

        // =========================
        //  TREM 4
        // =========================
        case 4:
            if (y == 150 && x < 270)
            {
                if (!inReg4 && x == 180)
                {
                    lockRegions({4});
                    inReg4 = true;
                }
                if (!inReg7 && x == 250)
                {
                    lockRegions({7});
                    inReg7 = true;
                }
                if (x == 220 && inReg3)
                {
                    unlockRegions({3});
                    inReg3 = false;
                }
                right();
            }
            else if (x == 270 && y < 280)
            {
                if (y == 170 && inReg4)
                {
                    unlockRegions({4});
                    inReg4 = false;
                }
                down();
            }
            else if (x > 130 && y == 280)
            {
                if (x == 250 && inReg7)
                {
                    unlockRegions({7});
                    inReg7 = false;
                }
                left();
            }
            else
            {
                if (x == 130 && y == 170 && !inReg3)
                {
                    lockRegions({3});
                    inReg3 = true;
                }
                up();
            }

            // Travamento preventivo de cruzamento completo:
            if (!inReg3 && !inReg4 && !inReg7 && x >= 130 && x <= 270 && y >= 150 && y <= 280)
            {
                lockRegions({3, 4, 7});
                inReg3 = inReg4 = inReg7 = true;
            }
            break;

        // =========================
        //  TREM 5
        // =========================
        case 5:
            if (y == 150 && x < 410)
            {
                if (x == 290 && inReg7)
                {
                    unlockRegions({7});
                    inReg7 = false;
                }
                if (x == 320 && !inReg6)
                {
                    lockRegions({6});
                    inReg6 = true;
                }
                if (x == 360 && inReg5)
                {
                    unlockRegions({5});
                    inReg5 = false;
                }
                right();
            }
            else if (x == 410 && y < 280)
            {
                if (y == 170 && inReg6)
                {
                    unlockRegions({6});
                    inReg6 = false;
                }
                down();
            }
            else if (x > 270 && y == 280)
            {
                if (x == 290 && !inReg7)
                {
                    lockRegions({7});
                    inReg7 = true;
                }
                left();
            }
            else
            {
                if (x == 270 && y == 170 && !inReg5)
                {
                    lockRegions({5});
                    inReg5 = true;
                }
                up();
            }

            if (!inReg5 && !inReg6 && !inReg7 && x >= 270 && x <= 410 && y >= 150 && y <= 280)
            {
                lockRegions({5, 6, 7});
                inReg5 = inReg6 = inReg7 = true;
            }
            break;

        // =========================
        //  TREM 6
        // =========================
        case 6:
            if (y == 30 && x > 60)
            {
                // Alternando regiões 1 e 2 conforme o movimento:
                if (x == 360 && !inReg2)
                {
                    lockRegions({2});
                    inReg2 = true;
                }
                if (x == 340 && inReg2)
                {
                    unlockRegions({2});
                    inReg2 = false;
                }
                if (x == 220 && !inReg1)
                {
                    lockRegions({1});
                    inReg1 = true;
                }
                if (x == 200 && inReg1)
                {
                    unlockRegions({1});
                    inReg1 = false;
                }
                left();
            }
            else if (x == 60 && y < 280)
            {
                down();
            }
            else if (x < 480 && y == 280)
            {
                if (x == 250 && !inReg7)
                {
                    lockRegions({7});
                    inReg7 = true;
                }
                if (x == 270 && inReg7)
                {
                    unlockRegions({7});
                    inReg7 = false;
                }
                right();
            }
            else
            {
                up();
            }

            // Travamento duplo no cruzamento superior:
            if (!inReg1 && !inReg2 && x >= 200 && x <= 360 && y <= 30)
            {
                lockRegions({1, 2});
                inReg1 = inReg2 = true;
            }
            break;

        default:
            break;
        }

        // Atualiza posição na interface (sinal Qt):
        emit updateGUI(ID, x, y);

        // Espera proporcional à velocidade:
        QThread::msleep(velocidade);
    }
} */

#include "trem.h"
#include "regioes.h"
#include <QtCore>

// ================================================
// Implementação da Classe Trem
// ================================================

// Construtor
Trem::Trem(int ID, int x, int y) {
    this->ID = ID;
    this->x = x;
    this->y = y;
    this->velocidade = 100; // valor padrão intermediário.
}

// Define a velocidade do trem (ajustada pelo slider):
void Trem::setVelocidade(int velocidade) {
    if (velocidade == 0) {
        this->velocidade = 1000000;  // trem parado.
    } else {
        this->velocidade = 200 - velocidade; // menor valor → mais rápido.
    }
}

// Métodos de movimento
void Trem::right() { x += 10; }
void Trem::left()  { x -= 10; }
void Trem::up()    { y -= 10; }
void Trem::down()  { y += 10; }

// ====================================================
// Função principal de execução da thread (loop do trem):
// ====================================================
void Trem::run() {
    while (true) {
        switch (ID) {

        // =====================================================
        // Trem 1
        // =====================================================
        case 1:
            if (y == 30 && x < 200) {
                if (x == 180) regiao1.lock();      // entra na região crítica 1.
                right();
            } else if (x == 200 && y < 150) {
                if (y == 130) regiao3.lock();      // entra na região crítica 3.
                down();
            } else if (x > 60 && y == 150) {
                if (x == 180) regiao1.unlock();    // sai da região 1;
                if (x == 110) regiao3.unlock();    // sai da região 3.
                left();
            } else {
                up();
            }
            emit updateGUI(ID, x, y);
            break;

        // =====================================================
        // Trem 2
        // =====================================================
        case 2:
            if (y == 30 && x < 340) {
                if (x == 220) regiao1.unlock();    // sai da 1;
                if (x == 320) regiao2.lock();      // entra na 2.
                right();
            } else if (x == 340 && y < 150) {
                if (y == 130) regiao5.lock();      // entra na 5.
                down();
            } else if (x > 200 && y == 150) {
                if (x == 320) regiao2.unlock();    // sai da 2;
                if (x == 290) regiao4.lock();      // entra na 4;
                if (x == 250) regiao5.unlock();    // sai da 5;
                if (x == 220) regiao1.lock();      // entra na 1.
                left();
            } else {
                up();
            }
            emit updateGUI(ID, x, y);
            break;

        // =====================================================
        // Trem 3
        // =====================================================
        case 3:
            if (y == 30 && x < 470) {
                if (x == 450) regiao2.unlock();
                right();
            } else if (x == 470 && y < 150) {
                if (y == 130) regiao6.lock();
                down();
            } else if (x > 340 && y == 150) {
                if (x == 450) regiao2.lock();
                if (x == 370) regiao4.unlock();
                if (x == 360) regiao6.unlock();
                left();
            } else {
                up();
            }
            emit updateGUI(ID, x, y);
            break;

        // =====================================================
        // Trem 4
        // =====================================================
        case 4:
            if (y == 150 && x < 280) {
                if (x == 260) regiao3.unlock();
                right();
            } else if (x == 280 && y < 270) {
                if (y == 250) regiao7.lock();
                down();
            } else if (x > 150 && y == 270) {
                if (x == 260) regiao7.unlock();
                left();
            } else {
                if (y == 160) regiao3.lock();
                up();
            }
            emit updateGUI(ID, x, y);
            break;

        // =====================================================
        // Trem 5
        // =====================================================
        case 5:
            if (y == 150 && x < 520) {
                if (x == 500) regiao4.unlock();
                right();
            } else if (x == 520 && y < 270) {
                if (y == 250) regiao7.lock();
                down();
            } else if (x > 280 && y == 270) {
                if (x == 500) regiao7.unlock();
                if (x == 490) regiao4.lock();
                left();
            } else {
                up();
            }
            emit updateGUI(ID, x, y);
            break;

        // =====================================================
        // Trem 6
        // =====================================================
        case 6:
            if (y == 150 && x < 650) {
                right();
            } else if (x == 650 && y < 270) {
                down();
            } else if (x > 520 && y == 270) {
                left();
            } else {
                up();
            }
            emit updateGUI(ID, x, y);
            break;
        }

        // Delay proporcional à velocidade:
        msleep(velocidade);
    }
}