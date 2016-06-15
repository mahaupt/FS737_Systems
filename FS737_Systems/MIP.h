//
//  MIP.hpp
//  FS737_Systems
//
//  Created by Marcel Haupt on 15.06.16.
//  Copyright © 2016 Marcel Haupt. All rights reserved.
//

#ifndef MIP_hpp
#define MIP_hpp

#include "Panel.h"
#include "FSToolbox/FSIcm.h"
#include "FSToolbox/LightController.h"
#include "FSToolbox/TimerManager.h"
#include "FSToolbox/Timer.h"

namespace fssystems
{
    using fstoolbox::FSIcm;
    using fsinterface::FSIID;
    using fstoolbox::LightController;
    using fstoolbox::TimerManager;
    using fstoolbox::Timer;
    
    class MIP : public Panel
    {
    private:
        static MIP * instance;
        
    public:
        MIP();
        static void fsiOnVarReceive(FSIID id);
    };
    
}

#endif /* MIP_hpp */
