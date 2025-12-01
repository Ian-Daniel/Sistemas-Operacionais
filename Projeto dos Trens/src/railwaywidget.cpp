#include "railwaywidget.h"
#include "layoutconstants.h"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QFont>

RailwayWidget::RailwayWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(700, 350);
    setStyleSheet("background-color: white;");
}

void RailwayWidget::setTrainPositions(const QVector<QPoint>& positions) {
    m_trainPositions = positions;
    update(); // Solicita repintura após atualização das posições.
}

void RailwayWidget::setTrainColors(const QVector<QColor>& colors) {
    m_trainColors = colors;
}

void RailwayWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);

    drawRailwayNetwork(painter);
    drawTrains(painter);
}

void RailwayWidget::drawRailwayNetwork(QPainter& painter) {
    // Estilo dos trilhos:
    QPen trackPen(Qt::blue, 5);
    trackPen.setCapStyle(Qt::RoundCap);
    painter.setPen(trackPen);

    using namespace Layout;

    // Desenho da malha ferroviária:
    painter.drawLine(P_TipLeft,  P_TopLeft);
    painter.drawLine(P_TipLeft,  P_BotLeft);
    painter.drawLine(P_TipLeft,  P_MidLeft);
    painter.drawLine(P_TopLeft,  P_BotLeft);
    painter.drawLine(P_TopRight, P_BotRight);
    painter.drawLine(P_TopLeft,  P_TopRight);
    painter.drawLine(P_MidLeft,  P_MidRight);
    painter.drawLine(P_BotLeft,  P_BotRight);
    painter.drawLine(P_TipRight, P_TopRight);
    painter.drawLine(P_TipRight, P_BotRight);
    painter.drawLine(P_MidRight, P_TipRight);
}

void RailwayWidget::drawTrains(QPainter& painter) {
    painter.setRenderHint(QPainter::Antialiasing);

    for (int i = 0; i < m_trainPositions.size(); i++) {
        if (i >= m_trainColors.size())
            break;

        QPoint pos = m_trainPositions[i];
        QColor color = m_trainColors[i];

        const int trainSize = 20;
        QRectF trainRect(
            pos.x() - trainSize / 2,
            pos.y() - trainSize / 2,
            trainSize,
            trainSize
        );

        // Corpo do trem:
        painter.setBrush(QBrush(color));
        painter.setPen(QPen(Qt::black, 2));
        painter.drawRect(trainRect);

        // Identificador do trem:
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 10, QFont::Bold));
        painter.drawText(trainRect, Qt::AlignCenter, QString::number(i));
    }
}