//
//  VOICEREC.cpp
//  FS737_Systems
//
//  Created by Marcel Haupt on 15.06.16.
//  Copyright © 2016 Marcel Haupt. All rights reserved.
//

#include "VOICEREC.h"


namespace fssystems
{
    VOICEREC * VOICEREC::instance = nullptr;
    
    VOICEREC::VOICEREC() :
        test_light_on_timer(3, enableTestLight),
        test_light_off_timer(1, disableTestLight)
    {
        instance = this;
        
        //debug variable
        is_debug = true;
        
        //starting FSI Client for IRS
        FSIcm::inst->RegisterCallback(fsiOnVarReceive);
        
        FSIID wanted_vars[] =
        {
            FSIID::MBI_CVR_TEST_SWITCH
        };
        FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));
        
        //standard values
        LightController::registerLight(FSIID::MBI_CVR_STATUS_LIGHT);
        
        //register timer
        TimerManager::addTimer(test_light_on_timer);
        TimerManager::addTimer(test_light_off_timer);
        
        FSIcm::inst->set<bool>(FSIID::MBI_CVR_LAMPTEST, false);
        FSIcm::inst->ProcessWrites();
        
    }
    
    
    void VOICEREC::fsiOnVarReceive(FSIID id)
    {
        if (id == FSIID::MBI_CVR_TEST_SWITCH)
        {
            if (FSIcm::inst->get<bool>(FSIID::MBI_CVR_TEST_SWITCH))
            {
                instance->debug("VOICEREC Test On");
                instance->test_light_on_timer.Start();
            }
            else
            {
                instance->debug("VOICEREC Test Off");
                instance->test_light_on_timer.Reset();
                instance->test_light_off_timer.Reset();
                
                LightController::set(FSIID::MBI_CVR_STATUS_LIGHT, false);
                LightController::ProcessWrites();
            }
        }
    }
    
    
    void VOICEREC::enableTestLight() {
        LightController::set(FSIID::MBI_CVR_STATUS_LIGHT, true);
        LightController::ProcessWrites();
        
        //turn light off after 1 sec - actually false
        //instance->test_light_off_timer.Start();
    }
    
    void VOICEREC::disableTestLight() {
        LightController::set(FSIID::MBI_CVR_STATUS_LIGHT, false);
        LightController::ProcessWrites();
    }
}