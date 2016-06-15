#include "LED.h"

namespace fssystems
{
	LED * LED::instance = nullptr;

	LED::LED()
	{
		//debug variable
		is_debug = true;

		instance = this;

		//starting FSI Client for IRS
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);
		FSIID wanted_vars[] =
		{
			FSIID::MBI_LED_TEST_SWITCH,
            
            FSIID::FSI_LEADING_EDGE_FLAPS_LEFT_INBOARD_PERCENT,
            FSIID::FSI_LEADING_EDGE_FLAPS_RIGHT_INBOARD_PERCENT,
            FSIID::FSI_LEADING_EDGE_FLAPS_LEFT_OUTBOARD_PERCENT,
            FSIID::FSI_LEADING_EDGE_FLAPS_RIGHT_OUTBOARD_PERCENT
		};
		FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));


		//standard values
		LightController::registerLight(FSIID::MBI_LED_FLAPS_1_FULL_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_FLAPS_1_TRANSIT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_FLAPS_2_FULL_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_FLAPS_2_TRANSIT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_FLAPS_3_FULL_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_FLAPS_3_TRANSIT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_FLAPS_4_FULL_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_FLAPS_4_TRANSIT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_1_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_1_FULL_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_1_TRANSIT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_2_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_2_FULL_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_2_TRANSIT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_3_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_3_FULL_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_3_TRANSIT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_4_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_4_FULL_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_4_TRANSIT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_5_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_5_FULL_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_5_TRANSIT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_6_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_6_FULL_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_6_TRANSIT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_7_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_7_FULL_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_7_TRANSIT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_8_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_8_FULL_EXT_LIGHT);
		LightController::registerLight(FSIID::MBI_LED_SLATS_8_TRANSIT_LIGHT);

		FSIcm::inst->set<bool>(FSIID::MBI_LED_LAMPTEST, false);

		FSIcm::inst->ProcessWrites();
	}

	void LED::fsiOnVarReceive(FSIID id) {
		instance->onVarReceive(id);
	}

	void LED::onVarReceive(FSIID & id)
	{
		if (id == FSIID::MBI_LED_TEST_SWITCH)
		{
			bool led_switch = FSIcm::inst->get<bool>(FSIID::MBI_LED_TEST_SWITCH);
			if (led_switch)
			{
				debug("LED Test On");
			}
			else
			{
				debug("LED Test off");
			}

			//LED Test Lights
			LightController::set(FSIID::MBI_LED_FLAPS_1_FULL_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_FLAPS_1_TRANSIT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_FLAPS_2_FULL_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_FLAPS_2_TRANSIT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_FLAPS_3_FULL_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_FLAPS_3_TRANSIT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_FLAPS_4_FULL_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_FLAPS_4_TRANSIT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_1_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_1_FULL_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_1_TRANSIT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_2_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_2_FULL_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_2_TRANSIT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_3_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_3_FULL_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_3_TRANSIT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_4_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_4_FULL_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_4_TRANSIT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_5_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_5_FULL_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_5_TRANSIT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_6_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_6_FULL_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_6_TRANSIT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_7_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_7_FULL_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_7_TRANSIT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_8_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_8_FULL_EXT_LIGHT, led_switch);
			LightController::set(FSIID::MBI_LED_SLATS_8_TRANSIT_LIGHT, led_switch);
			LightController::ProcessWrites();
		}
        
        //flaps 1 und 2
        if (id == FSIID::FSI_LEADING_EDGE_FLAPS_LEFT_INBOARD_PERCENT) {
            byte flaps_left_inboard = FSIcm::inst->get<byte>(FSIID::FSI_LEADING_EDGE_FLAPS_LEFT_INBOARD_PERCENT);
            
            //full extended
            if (flaps_left_inboard >= 100) {
                LightController::set(FSIID::MBI_LED_FLAPS_1_FULL_EXT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_FLAPS_2_FULL_EXT_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_LED_FLAPS_1_FULL_EXT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_FLAPS_2_FULL_EXT_LIGHT, false);
            }
            LightController::ProcessWrites();
            debug("LED Left Flap: " + std::to_string(flaps_left_inboard));
        }
        
        //flaps 3 und 4
        if (id == FSIID::FSI_LEADING_EDGE_FLAPS_RIGHT_INBOARD_PERCENT) {
            byte flaps_right_inboard = FSIcm::inst->get<byte>(FSIID::FSI_LEADING_EDGE_FLAPS_RIGHT_INBOARD_PERCENT);
            
            //full extended
            if (flaps_right_inboard >= 100) {
                LightController::set(FSIID::MBI_LED_FLAPS_3_FULL_EXT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_FLAPS_4_FULL_EXT_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_LED_FLAPS_3_FULL_EXT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_FLAPS_4_FULL_EXT_LIGHT, false);
            }
            LightController::ProcessWrites();
            debug("LED Right Flap: " + std::to_string(flaps_right_inboard));
        }
        
        //slats 1 - 4
        if (id == FSIID::FSI_LEADING_EDGE_FLAPS_LEFT_OUTBOARD_PERCENT) {
            byte slats_left_outboard = FSIcm::inst->get<byte>(FSIID::FSI_LEADING_EDGE_FLAPS_LEFT_OUTBOARD_PERCENT);
            
            //full extended
            if (slats_left_outboard >= 100) {
                LightController::set(FSIID::MBI_LED_SLATS_1_FULL_EXT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_2_FULL_EXT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_3_FULL_EXT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_4_FULL_EXT_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_LED_SLATS_1_FULL_EXT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_2_FULL_EXT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_3_FULL_EXT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_4_FULL_EXT_LIGHT, false);
            }
            
            //extended
            if (slats_left_outboard > 0) {
                LightController::set(FSIID::MBI_LED_SLATS_1_EXT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_2_EXT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_3_EXT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_4_EXT_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_LED_SLATS_1_EXT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_2_EXT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_3_EXT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_4_EXT_LIGHT, false);
            }
            LightController::ProcessWrites();
            debug("LED Left Slat: " + std::to_string(slats_left_outboard));
        }
        
        //slats 1 - 4
        if (id == FSIID::FSI_LEADING_EDGE_FLAPS_RIGHT_OUTBOARD_PERCENT) {
            byte slats_right_outboard = FSIcm::inst->get<byte>(FSIID::FSI_LEADING_EDGE_FLAPS_RIGHT_OUTBOARD_PERCENT);
            
            //full extended
            if (slats_right_outboard >= 100) {
                LightController::set(FSIID::MBI_LED_SLATS_5_FULL_EXT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_6_FULL_EXT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_7_FULL_EXT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_8_FULL_EXT_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_LED_SLATS_5_FULL_EXT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_6_FULL_EXT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_7_FULL_EXT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_8_FULL_EXT_LIGHT, false);
            }
            
            //extended
            if (slats_right_outboard > 0) {
                LightController::set(FSIID::MBI_LED_SLATS_5_EXT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_6_EXT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_7_EXT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_8_EXT_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_LED_SLATS_5_EXT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_6_EXT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_7_EXT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_8_EXT_LIGHT, false);
            }
            LightController::ProcessWrites();
            debug("LED Right Slat: " + std::to_string(slats_right_outboard));
        }
	}
}