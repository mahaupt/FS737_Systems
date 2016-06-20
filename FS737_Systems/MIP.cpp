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
            FSIID::FSI_SPOILER_INDICATOR_RIGHT,
            
            //POTIS
            FSIID::MBI_MIP_CM2_INBD_BRT_POTI,
            FSIID::MBI_MIP_CM2_OUTBD_BRT_POTI,
            FSIID::MBI_MIP_CM1_LOWER_DU_BRT_POTI,
            FSIID::MBI_MIP_CM1_INBD_BRT_POTI,
            FSIID::MBI_MIP_CM1_OUTBD_BRT_POTI,
            FSIID::MBI_MIP_CM1_UPPER_DU_BRT_POTI,
            

            //FAILURES
            FSIID::MALFX_ANTI_SKID_INOP_ACTIVE,
            FSIID::MALFX_CM1_PFD_BLUE_FAIL_ACTIVE,
            FSIID::MALFX_CM1_PFD_GREEN_FAIL_ACTIVE,
            FSIID::MALFX_CM1_PFD_RED_FAIL_ACTIVE,
            FSIID::MALFX_CM1_ND_BLUE_FAIL_ACTIVE,
            FSIID::MALFX_CM1_ND_GREEN_FAIL_ACTIVE,
            FSIID::MALFX_CM1_ND_RED_FAIL_ACTIVE,
            FSIID::MALFX_CM2_PFD_BLUE_FAIL_ACTIVE,
            FSIID::MALFX_CM2_PFD_GREEN_FAIL_ACTIVE,
            FSIID::MALFX_CM2_PFD_RED_FAIL_ACTIVE,
            FSIID::MALFX_CM2_ND_BLUE_FAIL_ACTIVE,
            FSIID::MALFX_CM2_ND_GREEN_FAIL_ACTIVE,
            FSIID::MALFX_CM2_ND_RED_FAIL_ACTIVE
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
        /*if (id == FSIID::MBI_MIP_CM1_LOWER_DU_BRT_POTI) {
            unsigned int poti = FSIcm::inst->get<unsigned int>(FSIID::MBI_MIP_CM1_LOWER_DU_BRT_POTI);
            poti /= 4;
            poti = 255 - poti;
            byte brgt = (byte)instance->saturate(poti, 0, 255);
            FSIcm::inst->set<byte>(FSIID::INT_BRIGHTNESS_LDU, brgt);
        }
        if (id == FSIID::MBI_MIP_CM1_INBD_BRT_POTI) {
            unsigned int poti = FSIcm::inst->get<unsigned int>(FSIID::MBI_MIP_CM1_INBD_BRT_POTI);
            poti /= 4;
            poti = 255 - poti;
            byte brgt = (byte)instance->saturate(poti, 0, 255);
            FSIcm::inst->set<byte>(FSIID::INT_BRIGHTNESS_ND_CPT, brgt);
        }
        if (id == FSIID::MBI_MIP_CM1_OUTBD_BRT_POTI) {
            unsigned int poti = FSIcm::inst->get<unsigned int>(FSIID::MBI_MIP_CM1_OUTBD_BRT_POTI);
            poti /= 4;
            poti = 255 - poti;
            byte brgt = (byte)instance->saturate(poti, 0, 255);
            FSIcm::inst->set<byte>(FSIID::INT_BRIGHTNESS_PFD_CPT, brgt);
        }
        if (id == FSIID::MBI_MIP_CM1_UPPER_DU_BRT_POTI) {
            unsigned int poti = FSIcm::inst->get<unsigned int>(FSIID::MBI_MIP_CM1_UPPER_DU_BRT_POTI);
            poti /= 4;
            poti = 255 - poti;
            byte brgt = (byte)instance->saturate(poti, 0, 255);
            FSIcm::inst->set<byte>(FSIID::INT_BRIGHTNESS_EICAS, brgt);
        }*/
        if (id == FSIID::MBI_MIP_CM2_INBD_BRT_POTI) {
            unsigned int poti = FSIcm::inst->get<unsigned int>(FSIID::MBI_MIP_CM2_INBD_BRT_POTI);
            poti /= 4;
            poti = 255 - poti;
            byte brgt = (byte)instance->saturate(poti, 0, 255);
            FSIcm::inst->set<byte>(FSIID::INT_BRIGHTNESS_ND_FO, brgt);
            LightController::ProcessWrites();
        }
        if (id == FSIID::MBI_MIP_CM2_OUTBD_BRT_POTI) {
            unsigned int poti = FSIcm::inst->get<unsigned int>(FSIID::MBI_MIP_CM2_OUTBD_BRT_POTI);
            poti /= 4;
            poti = 255 - poti;
            byte brgt = (byte)instance->saturate(poti, 0, 255);
            FSIcm::inst->set<byte>(FSIID::INT_BRIGHTNESS_PFD_FO, brgt);
            LightController::ProcessWrites();
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

        //FAILURES
        if (id == FSIID::MALFX_ANTI_SKID_INOP_ACTIVE) {
            if (FSIcm::inst->get<bool>(FSIID::MALFX_ANTI_SKID_INOP_ACTIVE)) {
                LightController::set(FSIID::MBI_MIP_CENTER_ANTISKID_INOP_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_MIP_CENTER_ANTISKID_INOP_LIGHT, false);
            }
            LightController::ProcessWrites();
        }
        //CAPT PFD
        if (id == FSIID::MALFX_CM1_PFD_RED_FAIL_ACTIVE) {
            byte color_fail = FSIcm::inst->get<bool>(FSIID::FSI_COLOR_FAIL_PFD_CPT);
            if (FSIcm::inst->get<bool>(FSIID::MALFX_CM1_PFD_RED_FAIL_ACTIVE)) {
                color_fail |= 0b0001;
            }
            else {
                color_fail &= ~0b0001;
            }
            FSIcm::inst->set<byte>(FSIID::FSI_COLOR_FAIL_PFD_CPT, color_fail);
            LightController::ProcessWrites();
        }
        if (id == FSIID::MALFX_CM1_PFD_GREEN_FAIL_ACTIVE) {
            byte color_fail = FSIcm::inst->get<bool>(FSIID::FSI_COLOR_FAIL_PFD_CPT);
            if (FSIcm::inst->get<bool>(FSIID::MALFX_CM1_PFD_GREEN_FAIL_ACTIVE)) {
                color_fail |= 0b0010;
            }
            else {
                color_fail &= ~0b0010;
            }
            FSIcm::inst->set<byte>(FSIID::FSI_COLOR_FAIL_PFD_CPT, color_fail);
            LightController::ProcessWrites();
        }
        if (id == FSIID::MALFX_CM1_PFD_BLUE_FAIL_ACTIVE) {
            byte color_fail = FSIcm::inst->get<bool>(FSIID::FSI_COLOR_FAIL_PFD_CPT);
            if (FSIcm::inst->get<bool>(FSIID::MALFX_CM1_PFD_BLUE_FAIL_ACTIVE)) {
                color_fail |= 0b0100;
            }
            else {
                color_fail &= ~0b0100;
            }
            FSIcm::inst->set<byte>(FSIID::FSI_COLOR_FAIL_PFD_CPT, color_fail);
            LightController::ProcessWrites();
        }

        //CAPT ND
        if (id == FSIID::MALFX_CM1_ND_RED_FAIL_ACTIVE) {
            byte color_fail = FSIcm::inst->get<bool>(FSIID::FSI_COLOR_FAIL_ND_CPT);
            if (FSIcm::inst->get<bool>(FSIID::MALFX_CM1_ND_RED_FAIL_ACTIVE)) {
                color_fail |= 0b0001;
            }
            else {
                color_fail &= ~0b0001;
            }
            FSIcm::inst->set<byte>(FSIID::FSI_COLOR_FAIL_ND_CPT, color_fail);
            LightController::ProcessWrites();
        }
        if (id == FSIID::MALFX_CM1_ND_GREEN_FAIL_ACTIVE) {
            byte color_fail = FSIcm::inst->get<bool>(FSIID::FSI_COLOR_FAIL_ND_CPT);
            if (FSIcm::inst->get<bool>(FSIID::MALFX_CM1_ND_GREEN_FAIL_ACTIVE)) {
                color_fail |= 0b0010;
            }
            else {
                color_fail &= ~0b0010;
            }
            FSIcm::inst->set<byte>(FSIID::FSI_COLOR_FAIL_ND_CPT, color_fail);
            LightController::ProcessWrites();
        }
        if (id == FSIID::MALFX_CM1_ND_BLUE_FAIL_ACTIVE) {
            byte color_fail = FSIcm::inst->get<bool>(FSIID::FSI_COLOR_FAIL_ND_CPT);
            if (FSIcm::inst->get<bool>(FSIID::MALFX_CM1_ND_BLUE_FAIL_ACTIVE)) {
                color_fail |= 0b0100;
            }
            else {
                color_fail &= ~0b0100;
            }
            FSIcm::inst->set<byte>(FSIID::FSI_COLOR_FAIL_ND_CPT, color_fail);
            LightController::ProcessWrites();
        }

        //CO PFD
        if (id == FSIID::MALFX_CM2_PFD_RED_FAIL_ACTIVE) {
            byte color_fail = FSIcm::inst->get<bool>(FSIID::FSI_COLOR_FAIL_PFD_FO);
            if (FSIcm::inst->get<bool>(FSIID::MALFX_CM2_PFD_RED_FAIL_ACTIVE)) {
                color_fail |= 0b0001;
            }
            else {
                color_fail &= ~0b0001;
            }
            FSIcm::inst->set<byte>(FSIID::FSI_COLOR_FAIL_PFD_FO, color_fail);
            LightController::ProcessWrites();
        }
        if (id == FSIID::MALFX_CM2_PFD_GREEN_FAIL_ACTIVE) {
            byte color_fail = FSIcm::inst->get<bool>(FSIID::FSI_COLOR_FAIL_PFD_FO);
            if (FSIcm::inst->get<bool>(FSIID::MALFX_CM2_PFD_GREEN_FAIL_ACTIVE)) {
                color_fail |= 0b0010;
            }
            else {
                color_fail &= ~0b0010;
            }
            FSIcm::inst->set<byte>(FSIID::FSI_COLOR_FAIL_PFD_FO, color_fail);
            LightController::ProcessWrites();
        }
        if (id == FSIID::MALFX_CM2_PFD_BLUE_FAIL_ACTIVE) {
            byte color_fail = FSIcm::inst->get<bool>(FSIID::FSI_COLOR_FAIL_PFD_FO);
            if (FSIcm::inst->get<bool>(FSIID::MALFX_CM2_PFD_BLUE_FAIL_ACTIVE)) {
                color_fail |= 0b0100;
            }
            else {
                color_fail &= ~0b0100;
            }
            FSIcm::inst->set<byte>(FSIID::FSI_COLOR_FAIL_PFD_FO, color_fail);
            LightController::ProcessWrites();
        }

        //CO ND
        if (id == FSIID::MALFX_CM2_ND_RED_FAIL_ACTIVE) {
            byte color_fail = FSIcm::inst->get<bool>(FSIID::FSI_COLOR_FAIL_ND_FO);
            if (FSIcm::inst->get<bool>(FSIID::MALFX_CM2_ND_RED_FAIL_ACTIVE)) {
                color_fail |= 0b0001;
            }
            else {
                color_fail &= ~0b0001;
            }
            FSIcm::inst->set<byte>(FSIID::FSI_COLOR_FAIL_ND_FO, color_fail);
            LightController::ProcessWrites();
        }
        if (id == FSIID::MALFX_CM2_ND_GREEN_FAIL_ACTIVE) {
            byte color_fail = FSIcm::inst->get<bool>(FSIID::FSI_COLOR_FAIL_ND_FO);
            if (FSIcm::inst->get<bool>(FSIID::MALFX_CM2_ND_GREEN_FAIL_ACTIVE)) {
                color_fail |= 0b0010;
            }
            else {
                color_fail &= ~0b0010;
            }
            FSIcm::inst->set<byte>(FSIID::FSI_COLOR_FAIL_ND_FO, color_fail);
            LightController::ProcessWrites();
        }
        if (id == FSIID::MALFX_CM2_ND_BLUE_FAIL_ACTIVE) {
            byte color_fail = FSIcm::inst->get<bool>(FSIID::FSI_COLOR_FAIL_ND_FO);
            if (FSIcm::inst->get<bool>(FSIID::MALFX_CM2_ND_BLUE_FAIL_ACTIVE)) {
                color_fail |= 0b0100;
            }
            else {
                color_fail &= ~0b0100;
            }
            FSIcm::inst->set<byte>(FSIID::FSI_COLOR_FAIL_ND_FO, color_fail);
            LightController::ProcessWrites();
        }
    }
}