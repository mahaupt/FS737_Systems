//
//  MIP.cpp
//  FS737_Systems
//
//  Created by Marcel Haupt on 15.06.16.
//  Copyright © 2016 Marcel Haupt. All rights reserved.
//

#include "MIP.h"


//LE FLAP Lights are in the LED Panel
//Gear Lights and Gear Switch are in SYSTEMS_Gear

namespace fssystems
{
    MIP * MIP::instance = nullptr;
    
    MIP::MIP()
    {
        instance = this;
        
        //debug variable
        is_debug = true;
        
        //starting FSI Client for IRS
        FSIcm::inst->RegisterCallback(fsiOnVarReceive);
        
        FSIID wanted_vars[] =
        {
            //gear
            FSIID::FSI_GEAR_ACTUAL_LEFT,
            FSIID::FSI_GEAR_ACTUAL_NOSE,
            FSIID::FSI_GEAR_ACTUAL_RIGHT,

            //POTIS for Brightness
            FSIID::MBI_MIP_CM1_LOWER_DU_BRT_POTI,
            FSIID::MBI_MIP_CM1_INBD_BRT_POTI,
            FSIID::MBI_MIP_CM1_OUTBD_BRT_POTI,
            FSIID::MBI_MIP_CM1_UPPER_DU_BRT_POTI,
            FSIID::MBI_MIP_CM2_INBD_BRT_POTI,
            FSIID::MBI_MIP_CM2_OUTBD_BRT_POTI,

            //LE FLAPS EXT -> LED
            //LE FLAPS TRNS -> LED

            //Speedbrake
            FSIID::FSI_SPOILER_INDICATOR_LEFT,
            FSIID::FSI_SPOILER_INDICATOR_RIGHT
        };
        FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));
        
        //standard values
        LightController::registerLight(FSIID::MBI_MIP_CM1_AFDS_FMC_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM1_FIRE_WARNING_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM1_MASTER_CAUTION_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM1_STAB_OUT_OF_TRIM_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM1_AFDS_AUTOPILOT_RED_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM1_AFDS_AUTOTHRUST_RED_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM1_BELOW_G_S_P_INHIBIT_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM1_AFDS_AUTOPILOT_AMBER_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM1_SPEED_BRAKE_EXTENDED_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM1_AFDS_AUTOTHRUST_AMBER_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM1_SPEED_BRAKE_DO_NOT_ARM_LIGHT);
        
        LightController::registerLight(FSIID::MBI_MIP_CM2_AFDS_FMC_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM2_GPWS_INOP_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM2_SIXPACK_ENG_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM2_SIXPACK_HYD_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM2_FIRE_WARNING_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM2_SIXPACK_DOORS_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM2_MASTER_CAUTION_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM2_SIXPACK_AIR_COND_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM2_SIXPACK_ANTI_ICE_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM2_SIXPACK_OVERHEAD_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM2_AFDS_AUTOPILOT_RED_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM2_AFDS_AUTOTHRUST_RED_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM2_BELOW_G_S_P_INHIBIT_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM2_AFDS_AUTOPILOT_AMBER_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM2_SPEEDBRAKES_EXTENDED_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CM2_AFDS_AUTOTHRUST_AMBER_LIGHT);
        
        LightController::registerLight(FSIID::MBI_MIP_CENTER_MFD_ENG_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_MFD_SYS_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_ANTISKID_INOP_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_AUTOBREAK_DISARM_LIGHT);
        
        FSIcm::inst->set<bool>(FSIID::MBI_MIP_CM1_LAMPTEST, false);
        FSIcm::inst->set<bool>(FSIID::MBI_MIP_CM2_LAMPTEST, false);
        FSIcm::inst->set<bool>(FSIID::MBI_MIP_CENTER_LAMPTEST, false);
        FSIcm::inst->ProcessWrites();
        
    }
    
    
    void MIP::fsiOnVarReceive(FSIID id)
    {

        //DISPLAY BRIGHTNESS
        if (id == FSIID::MBI_MIP_CM1_LOWER_DU_BRT_POTI) {
            unsigned int poti = FSIcm::inst->get<unsigned int>(FSIID::MBI_MIP_CM1_LOWER_DU_BRT_POTI);
            instance->debug("MIP Lower DU BRT: " + std::to_string(poti));
        }


        //SPOILER
        if (id == FSIID::FSI_SPOILER_INDICATOR_LEFT || id == FSIID::FSI_SPOILER_INDICATOR_RIGHT) {
            if (FSIcm::inst->get<float>(FSIID::FSI_SPOILER_INDICATOR_LEFT) > 0 || FSIcm::inst->get<float>(FSIID::FSI_SPOILER_INDICATOR_RIGHT) > 0) {
                LightController::set(FSIID::MBI_MIP_CM1_SPEED_BRAKE_EXTENDED_LIGHT, true);
                LightController::set(FSIID::MBI_MIP_CM2_SPEEDBRAKES_EXTENDED_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_MIP_CM1_SPEED_BRAKE_EXTENDED_LIGHT, false);
                LightController::set(FSIID::MBI_MIP_CM2_SPEEDBRAKES_EXTENDED_LIGHT, false);
            }
            LightController::ProcessWrites();
        }
    }
}