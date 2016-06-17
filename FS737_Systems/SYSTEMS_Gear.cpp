#include "SYSTEMS_Gear.h"


namespace fssystems
{

    SYSTEMS_Gear * SYSTEMS_Gear::instance;

    SYSTEMS_Gear::SYSTEMS_Gear()
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

            FSIID::MBI_MIP_CENTER_GEAR_SWITCH_DOWN_POS,
            FSIID::MBI_MIP_CENTER_GEAR_SWITCH_OFF_POS,
            FSIID::MBI_MIP_CENTER_GEAR_SWITCH_UP_POS,

            FSIID::FSI_PLANE_ON_GROUND
        };
        FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));

        LightController::registerLight(FSIID::MBI_MIP_CENTER_GEAR_LEFT_LOCKED_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_GEAR_NOSE_LOCKED_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_GEAR_LEFT_TRANSIT_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_GEAR_NOSE_TRANSIT_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_GEAR_RIGHT_LOCKED_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_GEAR_RIGHT_TRANSIT_LIGHT);

        //disable magnet
        FSIcm::inst->set(FSIID::MBI_MIP_CENTER_GEAR_LEVER_LOCK_MAGNET, false);

        FSIcm::inst->ProcessWrites();
    }



    void SYSTEMS_Gear::fsiOnVarReceive(FSIID id) {

        //gear to up position
        if (id == FSIID::MBI_MIP_CENTER_GEAR_SWITCH_UP_POS) {
            if (FSIcm::inst->get<bool>(FSIID::MBI_MIP_CENTER_GEAR_SWITCH_UP_POS)) {
                FSIcm::inst->set<bool>(FSIID::FSI_GEAR_COMMANDED, false);
            }
        }

        //gear to down pos
        if (id == FSIID::MBI_MIP_CENTER_GEAR_SWITCH_DOWN_POS) {
            if (FSIcm::inst->get<bool>(FSIID::MBI_MIP_CENTER_GEAR_SWITCH_DOWN_POS)) {
                FSIcm::inst->set<bool>(FSIID::FSI_GEAR_COMMANDED, true);
            }
        }

        //magent
        if (id == FSIID::FSI_PLANE_ON_GROUND) {
            //on GND and Gear Lever is DOWN
            if (FSIcm::inst->get<bool>(FSIID::FSI_PLANE_ON_GROUND) && FSIcm::inst->get<bool>(FSIID::MBI_MIP_CENTER_GEAR_SWITCH_DOWN_POS)) {
                FSIcm::inst->set(FSIID::MBI_MIP_CENTER_GEAR_LEVER_LOCK_MAGNET, true);
            } else {
                FSIcm::inst->set(FSIID::MBI_MIP_CENTER_GEAR_LEVER_LOCK_MAGNET, false);
            }
        }


        //MIP Lights
        if (id == FSIID::FSI_GEAR_ACTUAL_LEFT) {
            float gear_left = FSIcm::inst->get<float>(FSIID::FSI_GEAR_ACTUAL_LEFT);

            //transit
            if (gear_left > 0 && gear_left < 16383) {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_LEFT_TRANSIT_LIGHT, true);
            }
            else {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_LEFT_TRANSIT_LIGHT, false);
            }

            //locked
            if (gear_left == 16383) {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_LEFT_LOCKED_LIGHT, true);
            }
            else {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_LEFT_LOCKED_LIGHT, false);
            }
            LightController::ProcessWrites();
        }
        if (id == FSIID::FSI_GEAR_ACTUAL_RIGHT) {
            float gear_right = FSIcm::inst->get<float>(FSIID::FSI_GEAR_ACTUAL_RIGHT);

            //transit
            if (gear_right > 0 && gear_right < 16383) {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_RIGHT_TRANSIT_LIGHT, true);
            }
            else {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_RIGHT_TRANSIT_LIGHT, false);
            }

            //locked
            if (gear_right == 16383) {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_RIGHT_LOCKED_LIGHT, true);
            }
            else {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_RIGHT_LOCKED_LIGHT, false);
            }
            LightController::ProcessWrites();
        }
        if (id == FSIID::FSI_GEAR_ACTUAL_NOSE) {
            float gear_nose = FSIcm::inst->get<float>(FSIID::FSI_GEAR_ACTUAL_NOSE);

            //transit
            if (gear_nose > 0 && gear_nose < 16383) {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_NOSE_TRANSIT_LIGHT, true);
            }
            else {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_NOSE_TRANSIT_LIGHT, false);
            }

            //locked
            if (gear_nose == 16383) {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_NOSE_LOCKED_LIGHT, true);
            }
            else {
                LightController::set(FSIID::MBI_MIP_CENTER_GEAR_NOSE_LOCKED_LIGHT, false);
            }
            LightController::ProcessWrites();
        }
    }

}