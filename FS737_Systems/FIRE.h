//
//  FIRE.hpp
//  FS737_Systems
//
//  Created by Marcel Haupt on 15.06.16.
//  Copyright © 2016 Marcel Haupt. All rights reserved.
//

#ifndef FIRE_hpp
#define FIRE_hpp

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
    
    class FIRE : public Panel
    {
    private:
        static FIRE * instance;
        
    public:
        FIRE();
        static void fsiOnVarReceive(FSIID id);
    };
    
}


#endif /* FIRE_hpp */
