#include "trem.h"
#include "mainwindow.h"
#include <QtCore>
#include <semaphore.h>

// Semáforos das regiões críticas:
extern sem_t semaforos[11];

Trem::Trem(int ID, int x, int y, int vel)
{
    this->ID = ID;
    this->x = x;
    this->y = y;
    setVel(vel); // Limita velocidade.
}

void Trem::run()
{
    while (true)
    {
        if (vel > 0)
        {
            switch (ID)
            {

            // Trem 1 (verde):
            case 1:
                if (x == 280 && y == 40)
                {
                    sem_wait(&semaforos[0]);
                    sem_wait(&semaforos[2]);
                    x += 10;
                }
                else if (x == 290 && y == 40)
                    x += 10;
                else if (x == 300 && y == 40)
                    x += 10;
                else if (x == 310 && y == 40)
                {
                    y += 10;
                    sem_post(&semaforos[0]);
                    sem_post(&semaforos[2]);
                }
                else if (x == 310 && y == 130)
                {
                    sem_wait(&semaforos[2]);
                    y += 10;
                }
                else if (x == 310 && y == 140)
                    y += 10;
                else if (x == 310 && y == 150)
                    y += 10;
                else if (x == 310 && y == 160)
                {
                    x -= 10;
                    sem_post(&semaforos[2]);
                }
                else if (y == 40 && x < 310)
                    x += 10;
                else if (x == 310 && y < 160)
                    y += 10;
                else if (x > 40 && y == 160)
                    x -= 10;
                else
                    y -= 10;
                emit updateGUI(ID, x, y, vel);
                break;

            // Trem 2 (vermelho):
            case 2:
                if (x == 340 && y == 160)
                {
                    sem_wait(&semaforos[0]);
                    x -= 10;
                }
                else if (x == 330 && y == 160)
                    x -= 10;
                else if (x == 320 && y == 160)
                    x -= 10;
                else if (x == 310 && y == 160)
                {
                    y -= 10;
                    sem_post(&semaforos[0]);
                }
                else if (x == 550 && y == 40)
                {
                    sem_wait(&semaforos[1]);
                    sem_wait(&semaforos[5]);
                    x += 10;
                }
                else if (x == 560 && y == 40)
                    x += 10;
                else if (x == 570 && y == 40)
                    x += 10;
                else if (x == 580 && y == 40)
                {
                    y += 10;
                    sem_post(&semaforos[1]);
                    sem_post(&semaforos[5]);
                }
                else if (x == 470 && y == 160)
                {
                    sem_wait(&semaforos[3]);
                    x -= 10;
                }
                else if (x > 340 && x < 470 && y == 160)
                    x -= 10;
                else if (x == 340 && y == 160)
                {
                    y -= 10;
                    sem_post(&semaforos[3]);
                }
                else if (x == 580 && y == 130)
                {
                    sem_wait(&semaforos[3]);
                    sem_wait(&semaforos[4]);
                    y += 10;
                }
                else if (x == 580 && y == 140)
                    y += 10;
                else if (x == 580 && y == 150)
                    y += 10;
                else if (x == 580 && y == 160)
                {
                    x -= 10;
                    sem_post(&semaforos[3]);
                    sem_post(&semaforos[4]);
                }
                else if (y == 40 && x < 580)
                    x += 10;
                else if (x == 580 && y < 160)
                    y += 10;
                else if (x > 310 && y == 160)
                    x -= 10;
                else
                    y -= 10;
                emit updateGUI(ID, x, y, vel);
                break;

            // Trem 3 (azul):
            case 3:
                if (x == 740 && y == 160)
                {
                    sem_wait(&semaforos[1]);
                    sem_wait(&semaforos[5]);
                    x -= 10;
                }
                else if (x > 580 && x < 740 && y == 160)
                    x -= 10;
                else if (x == 580 && y == 160)
                    y -= 10;
                else if (x == 580 && y > 140 && y < 160)
                    y -= 10;
                else if (x == 580 && y == 140)
                {
                    x += 10;
                    sem_post(&semaforos[1]);
                    sem_post(&semaforos[5]);
                }
                else if (x == 610 && y == 160)
                {
                    sem_wait(&semaforos[1]);
                    x -= 10;
                }
                else if (x > 580 && x < 610 && y == 160)
                    x -= 10;
                else if (x == 580 && y == 160)
                    y -= 10;
                else if (x == 580 && y > 40 && y < 160)
                    y -= 10;
                else if (x == 580 && y == 40)
                {
                    x += 10;
                    sem_post(&semaforos[1]);
                }
                else if (y == 40 && x < 850)
                    x += 10;
                else if (x == 850 && y < 160)
                    y += 10;
                else if (x > 580 && y == 160)
                    x -= 10;
                else
                    y -= 10;
                emit updateGUI(ID, x, y, vel);
                break;

            // Trem 4 (laranja):
            case 4:
                if (x == 170 && y == 190)
                {
                    sem_wait(&semaforos[0]);
                    sem_wait(&semaforos[2]);
                    y -= 10;
                }
                else if (x == 170 && y > 160 && y < 190)
                    y -= 10;
                else if (x == 170 && y == 160)
                    x += 10;
                else if (x > 170 && x < 310 && y == 160)
                    x += 10;
                else if (x == 310 && y == 160)
                {
                    y += 10;
                    sem_post(&semaforos[0]);
                    sem_post(&semaforos[2]);
                }
                else if (x == 280 && y == 160)
                {
                    sem_wait(&semaforos[3]);
                    sem_wait(&semaforos[4]);
                    x += 10;
                }
                else if (x > 280 && x < 440 && y == 160)
                    x += 10;
                else if (x == 440 && y == 160)
                    y += 10;
                else if (x == 440 && y > 160 && y < 180)
                    y += 10;
                else if (x == 440 && y == 180)
                {
                    x -= 10;
                    sem_post(&semaforos[3]);
                    sem_post(&semaforos[4]);
                }
                else if (x == 410 && y == 160)
                {
                    sem_wait(&semaforos[6]);
                    x += 10;
                }
                else if (x == 420 && y == 160)
                    y += 10;
                else if (x == 420 && y > 160 && y < 280)
                    y += 10;
                else if (x == 420 && y == 280)
                {
                    x -= 10;
                    sem_post(&semaforos[6]);
                }
                else if (y == 160 && x < 440)
                    x += 10;
                else if (x == 440 && y < 280)
                    y += 10;
                else if (x > 170 && y == 280)
                    x -= 10;
                else
                    y -= 10;
                emit updateGUI(ID, x, y, vel);
                break;

            // Trem 5 (roxo):
            case 5:
                if (x == 440 && y == 190)
                {
                    sem_wait(&semaforos[4]);
                    sem_wait(&semaforos[5]);
                    y -= 10;
                }
                else if (x == 440 && y > 160 && y < 190)
                    y -= 10;
                else if (x == 440 && y == 160)
                    x += 10;
                else if (x > 440 && x < 600 && y == 160)
                    x += 10;
                else if (x == 600 && y == 160)
                {
                    y -= 10;
                    sem_post(&semaforos[4]);
                    sem_post(&semaforos[5]);
                }
                else if (x == 550 && y == 160)
                {
                    sem_wait(&semaforos[5]);
                    x += 10;
                }
                else if (x > 550 && x < 710 && y == 160)
                    x += 10;
                else if (x == 710 && y == 160)
                    y += 10;
                else if (x == 710 && y > 160 && y < 180)
                    y += 10;
                else if (x == 710 && y == 180)
                {
                    x -= 10;
                    sem_post(&semaforos[5]);
                }
                else if (x == 470 && y == 280)
                {
                    sem_wait(&semaforos[6]);
                    x -= 10;
                }
                else if (x > 420 && x < 470 && y == 280)
                    x -= 10;
                else if (x == 420 && y == 280)
                    y -= 10;
                else if (x == 420 && y > 160 && y < 280)
                    y -= 10;
                else if (x == 420 && y == 160)
                {
                    x += 10;
                    sem_post(&semaforos[6]);
                }
                else if (y == 160 && x < 710)
                    x += 10;
                else if (x == 710 && y < 280)
                    y += 10;
                else if (x > 440 && y == 280)
                    x -= 10;
                else
                    y -= 10;
                emit updateGUI(ID, x, y, vel);
                break;

            // Trem 6 (externo):
            case 6:
                if (x == 20 && y == 50)
                {
                    sem_wait(&semaforos[7]);
                    y -= 10;
                }
                else if (x == 20 && y == 40)
                    y -= 10;
                else if (x == 20 && y == 30)
                {
                    y -= 10;
                    sem_post(&semaforos[7]);
                }

                else if (y == 20 && x == 30)
                {
                    sem_wait(&semaforos[8]);
                    x += 10;
                }
                else if (y == 20 && x >= 40 && x <= 300)
                    x += 10;
                else if (y == 20 && x == 310)
                {
                    x += 10;
                    sem_post(&semaforos[8]);
                }

                else if (x == 870 && y == 50)
                {
                    sem_wait(&semaforos[9]);
                    y += 10;
                }
                else if (x == 870 && y == 60)
                    y += 10;
                else if (x == 870 && y == 150)
                {
                    y += 10;
                    sem_post(&semaforos[9]);
                }

                else if (y == 300 && x == 320)
                {
                    sem_wait(&semaforos[10]);
                    x -= 10;
                }
                else if (y == 300 && x <= 310 && x >= 50)
                    x -= 10;
                else if (y == 300 && x == 40)
                {
                    x -= 10;
                    sem_post(&semaforos[10]);
                }

                else if (x == 20 && y > 20)
                    y -= 10;
                else if (y == 20 && x < 870)
                    x += 10;
                else if (x == 870 && y < 300)
                    y += 10;
                else if (y == 300 && x > 20)
                    x -= 10;

                emit updateGUI(ID, x, y, vel);
                break;
            }

            msleep(200 - vel); // Controle de velocidade.
        }
        else
        {
            msleep(200); // Trem parado.
        }
    }
}

void Trem::setVel(int value)
{
    if (value < 0)
        vel = 0;
    else if (value > 200)
        vel = 200;
    else
        vel = value;
}

int Trem::getX() { return x; }
void Trem::setX(int x) { this->x = x; }

int Trem::getY() { return y; }
void Trem::setY(int y) { this->y = y; }