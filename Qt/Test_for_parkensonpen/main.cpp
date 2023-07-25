#include "mainwindow.h"
#include "Serial.h"
#include <windows.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    //初始化串口
    if(!OpenSerial(CBR_115200, 8, NOPARITY, ONESTOPBIT))
        return -1;

    //创建读串口子线程
    HANDLE hThreadRead = CreateThread(NULL, 0, ThreadRead, NULL, 0, NULL);


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
