#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QGridLayout>
#include <QVector>

class RailwayWidget;
class Train;
class RailwayNetwork;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTrainSpeedChanged(int trainId, int speed);
    void onTrainPositionChanged(int trainId, int x, int y);

private:
    void setupUI();
    void setupTrains();

    RailwayWidget* m_railwayWidget;
    RailwayNetwork* m_network;
    QVector<Train*> m_trains;
    QVector<QSlider*> m_speedSliders;
    QVector<QLabel*> m_speedLabels;
    QTimer* m_updateTimer;
};

#endif