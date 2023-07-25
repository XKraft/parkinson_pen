#ifndef SERIAL_H
#define SERIAL_H

#endif // SERIAL_H

#include "windows.h"
#include "iostream"
#include <QString>

extern float pitch, roll, yaw;

bool OpenSerial(int baud, int bytesize, int parity, int stopBits);
DWORD WINAPI ThreadRead(LPVOID lpParam);
