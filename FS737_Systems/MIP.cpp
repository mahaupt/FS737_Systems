//
//  MIP.cpp
//  FS737_Systems
//
//  Created by Marcel Haupt on 15.06.16.
//  Copyright © 2016 Marcel Haupt. All rights reserved.
//

#include "MIP.h"


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
            FSIID::FSI_GEAR_ACTUAL_LEFT,
            FSIID::FSI_GEAR_ACTUAL_NOSE,
            FSIID::FSI_GEAR_ACTUAL_RIGHT
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
        LightController::registerLight(FSIID::MBI_MIP_CENTER_LE_FLAPS_EXT_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_ANTISKID_INOP_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_AUTOBREAK_DISARM_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_GEAR_LEFT_LOCKED_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_GEAR_NOSE_LOCKED_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_LE_FLAPS_TRANSIT_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_GEAR_LEFT_TRANSIT_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_GEAR_NOSE_TRANSIT_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_GEAR_RIGHT_LOCKED_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_GEAR_RIGHT_TRANSIT_LIGHT);
        
        FSIcm::inst->set<bool>(FSIID::MBI_MIP_CM1_LAMPTEST, false);
        FSIcm::inst->set<bool>(FSIID::MBI_MIP_CM2_LAMPTEST, false);
        FSIcm::inst->set<bool>(FSIID::MBI_MIP_CENTER_LAMPTEST, false);
        FSIcm::inst->ProcessWrites();
        
    }
    
    
    void MIP::fsiOnVarReceive(FSIID id)
    {
        if (id == FSIID::FSI_GEAR_ACTUAL_LEFT) {
            float gear_left = FSIcm::inst->get<float>(FSIID::FSI_GEAR_ACTUAL_LEFT);
            
            //transit
            if (gear_left > 0 && gear_left < 16383) {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_LEFT_TRANSIT_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_LEFT_TRANSIT_LIGHT, false);
            }
            
            //locked
            if (gear_left == 16383) {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_LEFT_LOCKED_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_LEFT_LOCKED_LIGHT, false);
            }
        }
        if (id == FSIID::FSI_GEAR_ACTUAL_RIGHT) {
            float gear_right = FSIcm::inst->get<float>(FSIID::FSI_GEAR_ACTUAL_RIGHT);
            
            //transit
            if (gear_right > 0 && gear_right < 16383) {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_RIGHT_TRANSIT_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_RIGHT_TRANSIT_LIGHT, false);
            }
            
            //locked
            if (gear_right == 16383) {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_RIGHT_LOCKED_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_RIGHT_LOCKED_LIGHT, false);
            }
        }
        if (id == FSIID::FSI_GEAR_ACTUAL_NOSE) {
            float gear_nose = FSIcm::inst->get<float>(FSIID::FSI_GEAR_ACTUAL_NOSE);
            
            //transit
            if (gear_nose > 0 && gear_nose < 16383) {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_NOSE_TRANSIT_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_NOSE_TRANSIT_LIGHT, false);
            }
            
            //locked
            if (gear_nose == 16383) {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_NOSE_LOCKED_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_NOSE_LOCKED_LIGHT, false);
            }
        }
    }
}