#include "Serial.h"

HANDLE comHandle = INVALID_HANDLE_VALUE;
float pitch, roll, yaw;

bool Attitude_Data_decode(char bit);

//打开串口
bool OpenSerial(int baud, int bytesize, int parity, int stopBits)
{
    DCB dcb;
    COMMTIMEOUTS commtimeout;

    //打开串口
    //注意修改串口值
    comHandle = CreateFile(L"com7", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(comHandle == INVALID_HANDLE_VALUE)
    {
        std::cout << "error: No such com!" << std::endl;
        return false;
    }

    //设置串口状态属性
    GetCommState(comHandle, &dcb);
    dcb.BaudRate = baud;//波特率
    dcb.ByteSize = bytesize;//每个字节的有效位数
    dcb.Parity = parity;
    dcb.StopBits = stopBits;
    if(!SetCommState(comHandle, &dcb))
        std::cout << "error: set com state failed1" << std::endl;

    //设置超时时间
    commtimeout.ReadIntervalTimeout = MAXDWORD;
    commtimeout.ReadTotalTimeoutConstant = 0;
    commtimeout.ReadTotalTimeoutMultiplier = 0;
    commtimeout.WriteTotalTimeoutConstant = 1;
    commtimeout.WriteTotalTimeoutMultiplier = 1;
    if(!SetCommTimeouts(comHandle, &commtimeout))
        std::cout << "error: set timeout failed!" << std::endl;

    return true;
}

DWORD WINAPI ThreadRead(LPVOID lpParam)
{
    char szbuf[2] = {0};
    DWORD wRLen = 0;

    while(1)
    {
        if(ReadFile(comHandle, szbuf, 1, &wRLen, NULL))
        {
            if(wRLen == 1)
                Attitude_Data_decode(szbuf[0]);
//                if(Attitude_Data_decode(szbuf[0]))
//                    std::cout << "pitch:" << pitch << " roll:" << roll << " yaw:" << yaw << std::endl;
        }
    }
}

//解析姿态角数据包
bool Attitude_Data_decode(char bit)
{

    static int state = 0, i = 0;
    static char buf[16] = {0};
    static bool if_head = false;

    if(bit == 'S' && state == 0)
    {
        state = 1;
        i = 0;
    }
    else if(bit == 'A' && state == 1)
    {
        buf[i++] = '\n';
        pitch = QString(buf).toFloat();
        state = 2;
        i = 0;
    }
    else if(bit == 'B' && state == 2)
    {
        buf[i++] = '\n';
        roll = QString(buf).toFloat();
        state = 3;
        i = 0;
    }
    else if(bit == '\n' && state == 3)
    {
        buf[i++] = '\n';
        yaw = QString(buf).toFloat();
        state = 0;
        i = 0;
        return true;
    }
    else
    {
        buf[i++] = bit;
    }
    return false;

//废案：PICO发送消息包不可行，原因未知故放弃
//    if(!if_head)
//    {
//        if(bit == 0xFE && state == 0)
//        {
//            buf[state] = 0xFE;
//            ++state;
//        }
//        else if(bit == 0xFD && state == 1)
//        {
//            buf[state] = 0xFD;
//            ++state;
//            if_head = true;
//        }
//        else if(state == 1 && bit != 0xFD)
//        {
//            state = 0;
//        }
//    }
//    else
//    {
//        if(state == 14 && bit == 0xFF)
//        {
//            pitch = (float)((buf[2] << 24) | (buf[3] << 16) | (buf[4] << 8) | buf[5]);
//            roll = (float)((buf[6] << 24) | (buf[7] << 16) | (buf[8] << 8) | buf[9]);
//            yaw = (float)((buf[10] << 24) | (buf[11] << 16) | (buf[12] << 8) | buf[13]);
//            if_head = false;
//            state = 0;
//            return true;
//        }
//        buf[state] = bit;
//        ++state;
//    }
//    return false;
}
