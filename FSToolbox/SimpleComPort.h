//
//  SimpleComPort.hpp
//  FS737_Systems
//
//  Created by Marcel Haupt on 22.06.16.
//  Copyright © 2016 Marcel Haupt. All rights reserved.
//

#ifndef SimpleComPort_hpp
#define SimpleComPort_hpp

#if defined(_WIN32) || defined(_WIN64)
#include <windows>
#endif
#include <string>

namespace fstoolbox {
    typedef unsigned char byte;
    
#if defined(_WIN32) || defined(_WIN64)
    class SimpleComPort {
    private:
        
        HANDLE serialHandle;
    
    
    public:
        SimpleComPort(int comPort);
        ~SimpleComPort();
        
        void write(byte * data, size_t size);
    };
#endif
#if defined(__APPLE__) || defined(__linux__)
    class SimpleComPort {
    private:
        
        
    public:
        SimpleComPort(int comPort);
        ~SimpleComPort();
        
        void write(byte * data, size_t size);
    };

#endif
}

#endif /* SimpleComPort_hpp */
