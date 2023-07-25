#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QSplineSeries>
#include <QTimer>
#include <QString>
#include "Serial.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_action_start_triggered();
    void on_action_stop_triggered();

private:
    void UpdateSeriesDate();

    Ui::MainWindow *ui;

    QChart* chart1;
    QValueAxis* axisX;
    QValueAxis* axisY;
    QList<QSplineSeries*> series;

    QTimer* timer1;
    QTimer* timer2;
};
#endif // MAINWINDOW_H
