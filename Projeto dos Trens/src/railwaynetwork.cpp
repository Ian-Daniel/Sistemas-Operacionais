#include "railwaynetwork.h"
#include <QDebug>

RailwayNetwork::RailwayNetwork() {
    // Inicializa semáforos e marca regiões como livres:
    for (int i = 0; i < 7; ++i) {
        sem_init(&m_semaphores[i], 0, 1);
        m_owners[i] = -1;
    }
}

RailwayNetwork::~RailwayNetwork() {
    // Libera os semáforos no encerramento:
    for (int i = 0; i < 7; ++i) {
        sem_destroy(&m_semaphores[i]);
    }
}

bool RailwayNetwork::tryAcquireRegion(CriticalRegion region, int trainId) {
    int idx = static_cast<int>(region);

    // Tentativa não bloqueante de adquirir a região crítica:
    if (sem_trywait(&m_semaphores[idx]) == 0) {
        QMutexLocker locker(&m_mutexes[idx]);
        m_owners[idx] = trainId;
        return true;
    }
    return false;
}

void RailwayNetwork::releaseRegion(CriticalRegion region, int trainId) {
    int idx = static_cast<int>(region);

    QMutexLocker locker(&m_mutexes[idx]);

    // Libera a região apenas se for o proprietário atual:
    if (m_owners[idx] == trainId) {
        m_owners[idx] = -1;
        sem_post(&m_semaphores[idx]);
    }
}

QColor RailwayNetwork::getTrainColor(int trainId) const {
    static const QColor colors[] = {
        Qt::green,
        Qt::red,
        Qt::blue,
        QColor(255, 165, 0),
        Qt::yellow,
        QColor(128, 0, 128)
    };

    // Retorna cor válida ou preta caso o ID seja inválido:
    if (trainId >= 0 && trainId < 6)
        return colors[trainId];
    return Qt::black;
}