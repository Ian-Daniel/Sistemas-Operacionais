#ifndef RAILWAYWIDGET_H
#define RAILWAYWIDGET_H

#include <QWidget>
#include <QPoint>
#include <QColor>
#include <QVector>

class RailwayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RailwayWidget(QWidget *parent = nullptr);
    void setTrainPositions(const QVector<QPoint>& positions);
    void setTrainColors(const QVector<QColor>& colors);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void drawRailwayNetwork(class QPainter& painter);
    void drawTrains(class QPainter& painter);

    QVector<QPoint> m_trainPositions;
    QVector<QColor> m_trainColors;
};

#endif