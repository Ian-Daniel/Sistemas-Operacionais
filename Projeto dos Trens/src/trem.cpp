#include "train.h"
#include "railwaynetwork.h"
#include "uiLayout.h"
#include <QDebug>
#include <QMutexLocker>
#include <vector>
#include <cmath>
#include <algorithm>

Train::Train(int id, RailwayNetwork* network, QObject* parent)
    : QThread(parent)
    , m_id(id)
    , m_network(network)
    , m_speed(100)
    , m_running(true)
    , m_currentPathIndex(0)
    , m_heldRegions()
{
    using namespace Layout;

    // Caminhos definidos em sentido anti-horário:
    switch(id) {
        case 0: // Verde:
            m_x = P_TipLeft.x(); m_y = P_TipLeft.y();
            m_path = { {P_TipLeft.x(), P_TipLeft.y()}, {P_MidLeft.x(), P_MidLeft.y()}, {P_TopLeft.x(), P_TopLeft.y()} };
            break;

        case 1: // Vermelho:
            m_x = P_TopLeft.x(); m_y = P_TopLeft.y();
            m_path = { {P_TopLeft.x(), P_TopLeft.y()}, {P_MidLeft.x(), P_MidLeft.y()}, {P_MidRight.x(), P_MidRight.y()}, {P_TopRight.x(), P_TopRight.y()} };
            break;

        case 2: // Azul:
            m_x = P_TopRight.x(); m_y = P_TopRight.y();
            m_path = { {P_TopRight.x(), P_TopRight.y()}, {P_MidRight.x(), P_MidRight.y()}, {P_TipRight.x(), P_TipRight.y()} };
            break;

        case 3: // Laranja:
            m_x = P_TipLeft.x(); m_y = P_TipLeft.y();
            m_path = { {P_TipLeft.x(), P_TipLeft.y()}, {P_BotLeft.x(), P_BotLeft.y()}, {P_MidLeft.x(), P_MidLeft.y()} };
            break;

        case 4: // Amarelo:
            m_x = P_BotLeft.x(); m_y = P_BotLeft.y();
            m_path = { {P_BotLeft.x(), P_BotLeft.y()}, {P_BotRight.x(), P_BotRight.y()}, {P_MidRight.x(), P_MidRight.y()}, {P_MidLeft.x(), P_MidLeft.y()} };
            break;

        case 5: // Roxo:
            m_x = P_BotRight.x(); m_y = P_BotRight.y();
            m_path = { {P_BotRight.x(), P_BotRight.y()}, {P_TipRight.x(), P_TipRight.y()}, {P_MidRight.x(), P_MidRight.y()} };
            break;

        default:
            m_x = 0; m_y = 0;
            break;
    }
}

Train::~Train() {
    stopTrain();
    wait();
}

void Train::setSpeed(int speed) {
    QMutexLocker locker(&m_mutex);
    m_speed = qBound(0, speed, 200);
}

int Train::getSpeed() const {
    QMutexLocker locker(&m_mutex);
    return m_speed;
}

int Train::getId() const { return m_id; }

QPoint Train::getPosition() const {
    QMutexLocker locker(&m_mutex);
    return QPoint(m_x, m_y);
}

QColor Train::getColor() const {
    return m_network->getTrainColor(m_id);
}

void Train::stopTrain() {
    QMutexLocker locker(&m_mutex);
    m_running = false;
}

int mapSpeedToSleep(int speed) {
    return 200 - (speed * 198 / 200);
}

void moveTowards(int& x, int& y, int tx, int ty, int step) {
    int dx = tx - x;
    int dy = ty - y;
    double dist = std::sqrt(static_cast<double>(dx * dx + dy * dy));

    if (dist <= step) {
        x = tx;
        y = ty;
    } else {
        x += static_cast<int>((dx * step) / dist);
        y += static_cast<int>((dy * step) / dist);
    }
}

// Retorna regiões críticas necessárias para o próximo segmento:
std::vector<int> Train::getRequirementsForNextSegment(int targetIndex) {
    if (m_path.empty()) return {};

    int size = static_cast<int>(m_path.size());
    int prev = (targetIndex - 1 + size) % size;

    // Mapeamento de regiões críticas (Figura de referência):
    switch(m_id) {
        case 0: // Verde:
            if (prev == 0 && targetIndex == 1) return {2};
            if (prev == 1 && targetIndex == 2) return {0};
            break;

        case 1: // Vermelho:
            if (prev == 0 && targetIndex == 1) return {0};
            if (prev == 1 && targetIndex == 2) return {3};
            if (prev == 2 && targetIndex == 3) return {1};
            break;

        case 2: // Azul:
            if (prev == 0 && targetIndex == 1) return {1};
            if (prev == 1 && targetIndex == 2) return {4};
            break;

        case 3: // Laranja:
            if (prev == 1 && targetIndex == 2) return {5};
            if (prev == 2 && targetIndex == 0) return {2};
            break;

        case 4: // Amarelo:
            if (prev == 1 && targetIndex == 2) return {6};
            if (prev == 2 && targetIndex == 3) return {3};
            if (prev == 3 && targetIndex == 0) return {5};
            break;

        case 5: // Roxo:
            if (prev == 1 && targetIndex == 2) return {4};
            if (prev == 2 && targetIndex == 0) return {6};
            break;
    }
    return {};
}

void Train::run()
{
    using namespace Layout;

    auto acquire = [&](const std::vector<int>& regions) -> bool {
        for (size_t i = 0; i < regions.size(); ++i) {
            if (!m_network->tryAcquireRegion(static_cast<RailwayNetwork::CriticalRegion>(regions[i]), m_id)) {
                for (int j = static_cast<int>(i) - 1; j >= 0; --j)
                    m_network->releaseRegion(static_cast<RailwayNetwork::CriticalRegion>(regions[j]), m_id);
                return false;
            }
        }
        return true;
    };

    auto release = [&](int r) {
        m_network->releaseRegion(static_cast<RailwayNetwork::CriticalRegion>(r), m_id);
    };

    std::vector<int> held;
    const int STEP = 5;
    const int BRAKE_DIST = 55;

    while (true) {
        {
            QMutexLocker locker(&m_mutex);
            if (!m_running) break;
        }

        int speed;
        {
            QMutexLocker locker(&m_mutex);
            speed = m_speed;
        }

        if (speed == 0) {
            msleep(100);
            continue;
        }

        int x, y, idx;
        {
            QMutexLocker locker(&m_mutex);
            x = m_x; y = m_y;
            idx = m_currentPathIndex;
        }

        int tx = m_path[idx].first;
        int ty = m_path[idx].second;

        int dx = tx - x;
        int dy = ty - y;
        double dist = std::sqrt(static_cast<double>(dx * dx + dy * dy));

        // Verificação antecipada de regiões críticas:
        if (dist <= BRAKE_DIST) {
            int next = (idx + 1) % static_cast<int>(m_path.size());
            auto required = getRequirementsForNextSegment(next);

            std::vector<int> missing;
            for (int r : required)
                if (std::find(held.begin(), held.end(), r) == held.end())
                    missing.push_back(r);

            if (!missing.empty()) {
                if (acquire(missing))
                    held.insert(held.end(), missing.begin(), missing.end());
                else {
                    msleep(10);
                    continue;
                }
            }
        }

        // Chegada ao ponto-alvo:
        if (dist <= STEP) {
            int next = (idx + 1) % static_cast<int>(m_path.size());
            auto required = getRequirementsForNextSegment(next);

            std::vector<int> toRelease;
            for (int r : held)
                if (std::find(required.begin(), required.end(), r) == required.end())
                    toRelease.push_back(r);

            for (int r : toRelease) {
                release(r);
                held.erase(std::remove(held.begin(), held.end(), r), held.end());
            }

            {
                QMutexLocker locker(&m_mutex);
                m_currentPathIndex = next;
                m_x = tx;
                m_y = ty;
            }
            continue;
        }

        // Movimento contínuo:
        int nx = x, ny = y;
        moveTowards(nx, ny, tx, ty, STEP);

        {
            QMutexLocker locker(&m_mutex);
            m_x = nx; m_y = ny;
        }
        emit positionChanged(m_id, nx, ny);

        int sleep = mapSpeedToSleep(speed);
        if (sleep < 5) sleep = 5;
        msleep(sleep);
    }

    for (int r : held) release(r);
}