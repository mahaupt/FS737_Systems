//
//  VOICEREC.hpp
//  FS737_Systems
//
//  Created by Marcel Haupt on 15.06.16.
//  Copyright © 2016 Marcel Haupt. All rights reserved.
//

#ifndef VOICEREC_hpp
#define VOICEREC_hpp

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
    
    class VOICEREC : public Panel
    {
    private:
        static VOICEREC * instance;
        Timer test_light_timer;
        
    public:
        VOICEREC();
        static void fsiOnVarReceive(FSIID id);
        static void enableTestLight();
    };
    
}

#endif /* VOICEREC_hpp */
