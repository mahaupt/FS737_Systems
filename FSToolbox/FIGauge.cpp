//
//  FIGauge.cpp
//  FS737_Systems
//
//  Created by Marcel Haupt on 22.06.16.
//  Copyright © 2016 Marcel Haupt. All rights reserved.
//

#include "FIGauge.h"


namespace fstoolbox {
    FIGauge::FIGauge(int com) :
        comPort(com)
    {
        
    }
    
    void FIGauge::sendData(byte gaugeID, byte command, long value) {
        byte buffer[]={0x00,0x00,0x00,0x00,0x00,0x00};
        byte bytethree;
        int int2;
        long long1;
        buffer[1]=gaugeID;      // Gauge ID
        buffer[5]=0xff;    // Terminator
        
        long1 = std::abs((double)value);
        int2 = (int) (long1 >> 8);
        
        buffer[3] = ( (byte) long1 & 0xff) | 0x01;
        buffer[4] = ( (byte) int2 & 0xff) | 0x02;
        
        bytethree = command << 4;
        bytethree |= ( (byte) long1 & 0x01);
        bytethree |= ( (byte) int2 & 0x02);
        if (value < 0) bytethree |= 0x0C;
        else bytethree |= 0x08;
        
        buffer[2]=bytethree & 0xff;
        
        comPort.write(buffer, sizeof(buffer));
    }
    
    
    
}
