//
//  FIGauge.hpp
//  FS737_Systems
//
//  Created by Marcel Haupt on 22.06.16.
//  Copyright © 2016 Marcel Haupt. All rights reserved.
//

#ifndef FIGauge_hpp
#define FIGauge_hpp
#include <cmath>

#include "SimpleComPort.h"

namespace fstoolbox {
    typedef unsigned char byte;
    
    enum FIGAUGE_CMD : byte {
        FIG_WRITE_VALUE_1 = 5
    };
    enum FIGAUGE_ID : byte {
        FIG_APU_EGT = 0x01
    };
    
    class FIGauge {
    private:
        SimpleComPort comPort;
        
        void sendData(byte gaugeID, byte command, long value);
    public:
        FIGauge(int com);
    };
}

#endif /* FIGauge_hpp */
