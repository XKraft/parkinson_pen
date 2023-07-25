#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
    axisX->setRange(0, 30);
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
            if(s->count() < 1000)
            {
                s->append(QPointF(count * 0.03, AttitudeData[i]));
            }
            else
            {
                axisX->setRange((count - 1000) * 0.03, count * 0.03);
                QList<QPointF> v = s->points();
                v.append(QPointF(count * 0.03, AttitudeData[i]));
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
    timer1->start(30);
}

void MainWindow::on_action_stop_triggered()
{
    std::cout << "stop draw graph..." << std::endl;
    timer1->stop();
}
