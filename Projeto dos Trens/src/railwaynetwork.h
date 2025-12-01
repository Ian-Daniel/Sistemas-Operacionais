#ifndef RAILWAYNETWORK_H
#define RAILWAYNETWORK_H

#include <QObject>
#include <QMutex>
#include <QColor>
#include <semaphore.h>

class RailwayNetwork : public QObject
{
    Q_OBJECT

public:
    enum CriticalRegion {
        Region0 = 0,
        Region1 = 1,
        Region2 = 2,
        Region3 = 3,
        Region4 = 4,
        Region5 = 5,
        Region6 = 6
    };

    RailwayNetwork();
    ~RailwayNetwork();

    bool tryAcquireRegion(CriticalRegion region, int trainId);
    void releaseRegion(CriticalRegion region, int trainId);
    QColor getTrainColor(int trainId) const;

private:
    sem_t m_semaphores[7];
    QMutex m_mutexes[7];
    int m_owners[7];
};

#endif