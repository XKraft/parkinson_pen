#include "mainwindow.h"
#include <fstream>
#include <ctime>
#include "./ui_mainwindow.h"

#define DRAW_INTERVAL 30 //绘图两点间的时间间隔，单位ms
#define DRAW_MAX_DATALEN 1000 //绘图最大点的个数

const char* seriesname[3] = {"pitch", "roll", "yaw"};
Qt::GlobalColor color[3] = {Qt::red, Qt::blue, Qt::green};
float AttitudeData[3] = {0};
int count = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    chart1 = new QChart();
    chart1->setTitle((QString)"帕金森笔姿态角曲线");
    ui->widget_graph->setChart(chart1);
    ui->widget_graph->setRubberBand(QChartView::NoRubberBand);
    ui->widget_graph->setRenderHint(QPainter::Antialiasing);
    ui->widget_graph->show();

    axisX = new QValueAxis;
    axisY = new QValueAxis;
    axisX->setRange(0, DRAW_INTERVAL * DRAW_MAX_DATALEN / 1000.0);
    axisX->setLabelFormat(("%.2f"));
    axisX->setTitleText("时间/s");
    axisX->setGridLineVisible(false);
    axisX->setTickCount(2);
    axisY->setRange(-90, 90);
    axisY->setLabelFormat(("%.2f"));
    axisY->setTitleText("角度/°");

    chart1->addAxis(axisX, Qt::AlignBottom);
    chart1->addAxis(axisY, Qt::AlignLeft);
    chart1->legend()->setVisible(true);

    for(int i = 0; i < 3; ++i)
    {
        QSplineSeries* s = new QSplineSeries(this);
        s->setName(QString(seriesname[i]));
        chart1->addSeries(s);
        s->attachAxis(axisX);
        s->attachAxis(axisY);
        QPen pen;
        pen.setBrush(color[i]);
        pen.setColor(color[i]);
        s->setPen(pen);
        series.append(s);
    }

    timer1 = nullptr;
    timer2 = nullptr;

//    connect(ui->action_start, SIGNAL(triggered()), this, SLOT(on_action_start_triggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete axisX;
    delete axisY;
    delete chart1;
}

void MainWindow::UpdateSeriesDate()
{
    AttitudeData[0] = pitch;
    AttitudeData[1] = roll;
    AttitudeData[2] = yaw;

    ui->label_pitch->setText((QString)"Pitch:" + QString::number(AttitudeData[0]) + (QString)"°");
    ui->label_roll->setText((QString)"roll:" + QString::number(AttitudeData[1]) + (QString)"°");
    ui->label_yaw->setText((QString)"yaw:" + QString::number(AttitudeData[2]) + (QString)"°");

    int i = 0;
    foreach(QSplineSeries* s, series)
    {
        s->setVisible(true);
        if(s->isVisible())
        {
            if(s->count() < DRAW_MAX_DATALEN)
            {
                s->append(QPointF(count * (DRAW_INTERVAL / 1000.0), AttitudeData[i]));
            }
            else
            {
                axisX->setRange((count - DRAW_MAX_DATALEN) * (DRAW_INTERVAL / 1000.0), count * (DRAW_INTERVAL / 1000.0));
                QList<QPointF> v = s->points();
                v.append(QPointF(count * (DRAW_INTERVAL / 1000.0), AttitudeData[i]));
                v.pop_front();
                s->replace(v);
            }
        }
        i++;
    }

    count++;
}

void MainWindow::on_action_start_triggered()
{
    if(timer1 != nullptr)
    {
        delete timer1;
        timer1 = nullptr;
    }
    count = 0;
    axisX->setRange(0, DRAW_INTERVAL * DRAW_MAX_DATALEN / 1000.0);
    timer1 = new QTimer(this);
    std::cout << "start draw graph..." << std::endl;
    //清空数据
    foreach(QSplineSeries* s, series)
        {

            QVector<QPointF> v;
            s->replace(v);
        }
    MainWindow::connect(timer1, &QTimer::timeout, this, [=](){
        UpdateSeriesDate();
    });
    timer1->start(DRAW_INTERVAL);
}

void MainWindow::on_action_stop_triggered()
{
    if(timer1)
    {
        std::cout << "stop draw graph..." << std::endl;
        timer1->stop();
}
}

void MainWindow::on_action_continue_triggered()
{
    if(timer1)
    {
        std::cout << "continue draw graph..." << std::endl;
        timer1->start();
    }
}

void MainWindow::on_action_save_triggered()
{
    //获取当前时间
    time_t t = time(nullptr);
    struct tm* now = localtime(&t);
    now->tm_year += 1900;
    now->tm_mon += 1;
    std::string now_time = std::to_string(now->tm_year) + "_" + std::to_string(now->tm_mon) + "_" + std::to_string(now->tm_mday) + "_" + std::to_string(now->tm_hour) + "_" + std::to_string(now->tm_min) + "_" + std::to_string(now->tm_sec);

    //修改到自己想要保存的路径
    std::ofstream outfile("D:\\study\\parkinson_pen\\Qt\\Test_for_parkensonpen\\savedata\\" + now_time + ".txt", std::ios::app);
    if(!outfile.is_open())
    {
        std::cout << "unable open the file..." << std::endl;
    }
    int i = 0;
    QList<QPointF> v[3];
    foreach(QSplineSeries* s, series)
    {
        v[i] = s->points();
//        std::for_each(v.begin(), v.end(), [&](QPointF p){
//            outfile << i << " " << p.x() << " " << p.y() << std::endl;
//        });
        i++;
    }
    int len = v[0].count();
    for(int j = 0; j < len; j++)
    {
        for(int k = 0; k < 3; k++)
            if(k == 0)
                outfile << v[k][j].x() << " " << v[k][j].y() << " ";
            else
                outfile << v[k][j].y() << " ";
        outfile << std::endl;
    }
    outfile.close();
    std::cout << "savedata..." << std::endl;
}
