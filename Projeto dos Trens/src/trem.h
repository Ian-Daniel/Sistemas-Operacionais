#ifndef TRAIN_H
#define TRAIN_H

#include <QThread>
#include <QMutex>
#include <QPoint>
#include <QColor>
#include <vector>
#include <QObject>

class RailwayNetwork;

class Train : public QThread
{
    Q_OBJECT

public:
    Train(int id, RailwayNetwork* network, QObject* parent = nullptr);
    ~Train();

    void setSpeed(int speed);
    int getSpeed() const;
    int getId() const;
    QPoint getPosition() const;
    QColor getColor() const;
    void stopTrain();

signals:
    void positionChanged(int trainId, int x, int y);

protected:
    void run() override;

private:
    std::vector<int> getRequirementsForNextSegment(int targetIndex);
    
    int m_id;
    RailwayNetwork* m_network;
    
    mutable QMutex m_mutex;
    int m_speed;
    bool m_running;
    
    int m_x, m_y;
    std::vector<std::pair<int, int>> m_path;
    int m_currentPathIndex;
    std::vector<int> m_heldRegions;
};

#endif