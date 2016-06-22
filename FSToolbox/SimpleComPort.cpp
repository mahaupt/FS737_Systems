//
//  SimpleComPort.cpp
//  FS737_Systems
//
//  Created by Marcel Haupt on 22.06.16.
//  Copyright © 2016 Marcel Haupt. All rights reserved.
//

#include "SimpleComPort.h"

namespace fstoolbox {
    
#if defined(_WIN32) || defined(_WIN64)
    SimpleComPort::SimpleComPort(int comPort) {
        serialHandle = CreateFile("\\\\.\\COM" + std::to_str(comPort), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        
        // Do some basic settings
        DCB serialParams = { 0 };
        serialParams.DCBlength = sizeof(serialParams);
        
        GetCommState(serialHandle, &serialParams);
        serialParams.BaudRate = 38400;
        serialParams.ByteSize = 8;
        serialParams.StopBits = 0;
        serialParams.Parity = 0;
        SetCommState(serialHandle, &serialParams);
        
        // Set timeouts
        COMMTIMEOUTS timeout = { 0 };
        timeout.ReadIntervalTimeout = 50;
        timeout.ReadTotalTimeoutConstant = 50;
        timeout.ReadTotalTimeoutMultiplier = 50;
        timeout.WriteTotalTimeoutConstant = 50;
        timeout.WriteTotalTimeoutMultiplier = 10;
        
        SetCommTimeouts(serialHandle, &timeout);
    }
    
    SimpleComPort::~SimpleComPort() {
        CloseHandle(serialHandle);
    }

    void SimpleComPort::write(byte * data, size_t size) {
        
    }
#endif
#if defined(__APPLE__) || defined(__linux__)
    SimpleComPort::SimpleComPort(int comPort) {
    }
    SimpleComPort::~SimpleComPort() {
    }
    
    void SimpleComPort::write(byte * data, size_t size) {
    }
#endif
}