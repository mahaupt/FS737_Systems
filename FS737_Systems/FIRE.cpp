//
//  FIRE.cpp
//  FS737_Systems
//
//  Created by Marcel Haupt on 15.06.16.
//  Copyright © 2016 Marcel Haupt. All rights reserved.
//

#include "FIRE.h"


namespace fssystems
{
    FIRE * FIRE::instance = nullptr;
    
    FIRE::FIRE()
    {
        instance = this;
        
        //debug variable
        is_debug = true;
        
        //starting FSI Client for IRS
        FSIcm::inst->RegisterCallback(fsiOnVarReceive);
        
        FSIID wanted_vars[] =
        {
            FSIID::MBI_FIRE_ENGINES_EXT_TEST_1_SWITCH,
            FSIID::MBI_FIRE_ENGINES_EXT_TEST_2_SWITCH,
            FSIID::MBI_FIRE_TEST_FAULT_INOP_SWITCH,
            FSIID::MBI_FIRE_TEST_OVHT_FIRE_SWITCH
        };
        FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));
        
        //standard values
        LightController::registerLight(FSIID::MBI_FIRE_FAULT_LIGHT);
        LightController::registerLight(FSIID::MBI_FIRE_UNKNOWN_1_LIGHT);
        LightController::registerLight(FSIID::MBI_FIRE_UNKNOWN_2_LIGHT);
        LightController::registerLight(FSIID::MBI_FIRE_UNKNOWN_3_LIGHT);
        LightController::registerLight(FSIID::MBI_FIRE_WHEEL_WELL_LIGHT);
        LightController::registerLight(FSIID::MBI_FIRE_APU_DET_INOP_LIGHT);
        LightController::registerLight(FSIID::MBI_FIRE_ENG_1_OVERHEAT_LIGHT);
        LightController::registerLight(FSIID::MBI_FIRE_ENG_2_OVERHEAT_LIGHT);
        LightController::registerLight(FSIID::MBI_FIRE_EXTINGUISHER_TEST_L_LIGHT);
        LightController::registerLight(FSIID::MBI_FIRE_EXTINGUISHER_TEST_R_LIGHT);
        LightController::registerLight(FSIID::MBI_FIRE_L_BOTTLE_DISCHARGED_LIGHT);
        LightController::registerLight(FSIID::MBI_FIRE_APU_BOTTLE_DISCHARGED_LIGHT);
        LightController::registerLight(FSIID::MBI_FIRE_R_BOTTLE_DISCHARGED_LIGHT);
        LightController::registerLight(FSIID::MBI_FIRE_EXTINGUISHER_TEST_APU_LIGHT);
        
        FSIcm::inst->set<bool>(FSIID::MBI_FIRE_LAMPTEST, false);
        FSIcm::inst->ProcessWrites();
        
    }
    
    
    void FIRE::fsiOnVarReceive(FSIID id)
    {
        if (id == FSIID::MBI_FIRE_ENGINES_EXT_TEST_1_SWITCH || id == FSIID::MBI_FIRE_ENGINES_EXT_TEST_2_SWITCH) {
            bool test = FSIcm::inst->get<bool>(FSIID::MBI_FIRE_ENGINES_EXT_TEST_1_SWITCH) || FSIcm::inst->get<bool>(FSIID::MBI_FIRE_ENGINES_EXT_TEST_2_SWITCH);
            
            instance->debug("FIRE EXT TEST SWICH " + std::to_string((int)test));
            
            LightController::set(FSIID::MBI_FIRE_EXTINGUISHER_TEST_L_LIGHT, test);
            LightController::set(FSIID::MBI_FIRE_EXTINGUISHER_TEST_R_LIGHT, test);
            LightController::set(FSIID::MBI_FIRE_EXTINGUISHER_TEST_APU_LIGHT, test);
            LightController::ProcessWrites();
        }
        
        if (id == FSIID::MBI_FIRE_TEST_FAULT_INOP_SWITCH) {
            bool test = FSIcm::inst->get<bool>(FSIID::MBI_FIRE_TEST_FAULT_INOP_SWITCH);
            instance->debug("FIRE TEST FAULT INOP SWICH " + std::to_string((int)test));
            
            LightController::set(FSIID::MBI_FIRE_FAULT_LIGHT, test);
            LightController::set(FSIID::MBI_FIRE_APU_DET_INOP_LIGHT, test);
            LightController::ProcessWrites();
        }
        
        if (id == FSIID::MBI_FIRE_TEST_OVHT_FIRE_SWITCH) {
            bool test = FSIcm::inst->get<bool>(FSIID::MBI_FIRE_TEST_OVHT_FIRE_SWITCH);
            instance->debug("FIRE TEST OVHT SWICH " + std::to_string((int)test));
            
            LightController::set(FSIID::MBI_FIRE_WHEEL_WELL_LIGHT, test);
            LightController::set(FSIID::MBI_FIRE_ENG_1_OVERHEAT_LIGHT, test);
            LightController::set(FSIID::MBI_FIRE_ENG_2_OVERHEAT_LIGHT, test);
            LightController::set(FSIID::MBI_FIRE_UNKNOWN_1_LIGHT, test);
            LightController::set(FSIID::MBI_FIRE_UNKNOWN_2_LIGHT, test);
            LightController::set(FSIID::MBI_FIRE_UNKNOWN_3_LIGHT, test);
            LightController::ProcessWrites();
        }
    }
}