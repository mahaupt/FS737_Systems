#include "LED.h"

namespace fssystems
{
	LED * LED::instance = nullptr;

	LED::LED()
	{
		//debug variable
		is_debug = true;

		instance = this;
        
        //register timed function
        TimerManager::addTimedCallback(timedFunction);

		//starting FSI Client for IRS
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);
		FSIID wanted_vars[] =
		{
			FSIID::MBI_LED_TEST_SWITCH,
            
            FSIID::FSI_LEADING_EDGE_FLAPS_LEFT_INBOARD_PERCENT,
            FSIID::FSI_LEADING_EDGE_FLAPS_RIGHT_INBOARD_PERCENT,
            FSIID::FSI_LEADING_EDGE_FLAPS_LEFT_OUTBOARD_PERCENT,
            FSIID::FSI_LEADING_EDGE_FLAPS_RIGHT_OUTBOARD_PERCENT,

            //failures
            FSIID::MALFX_LE_FLAP_EXTENDED_LIGHT_OFF_ACTIVE,
            FSIID::MALFX_LE_FLAP_TRANSIT_LIGHT_ON_ACTIVE
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

        //MIP Lights
        LightController::registerLight(FSIID::MBI_MIP_CENTER_LE_FLAPS_TRANSIT_LIGHT);
        LightController::registerLight(FSIID::MBI_MIP_CENTER_LE_FLAPS_EXT_LIGHT);

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
            
            int led_override = (led_switch) ? 2 : 1;

			//LED Test Lights
			LightController::setOverride(FSIID::MBI_LED_FLAPS_1_FULL_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_FLAPS_1_TRANSIT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_FLAPS_2_FULL_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_FLAPS_2_TRANSIT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_FLAPS_3_FULL_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_FLAPS_3_TRANSIT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_FLAPS_4_FULL_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_FLAPS_4_TRANSIT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_1_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_1_FULL_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_1_TRANSIT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_2_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_2_FULL_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_2_TRANSIT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_3_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_3_FULL_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_3_TRANSIT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_4_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_4_FULL_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_4_TRANSIT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_5_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_5_FULL_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_5_TRANSIT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_6_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_6_FULL_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_6_TRANSIT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_7_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_7_FULL_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_7_TRANSIT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_8_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_8_FULL_EXT_LIGHT, led_override);
			LightController::setOverride(FSIID::MBI_LED_SLATS_8_TRANSIT_LIGHT, led_override);
			LightController::ProcessWrites();
		}
        
        //flaps 1 und 2
        if (id == FSIID::FSI_LEADING_EDGE_FLAPS_LEFT_INBOARD_PERCENT || id == FSIID::MALFX_LE_FLAP_EXTENDED_LIGHT_OFF_ACTIVE) {
            byte flaps_left_inboard = FSIcm::inst->get<byte>(FSIID::FSI_LEADING_EDGE_FLAPS_LEFT_INBOARD_PERCENT);
            
            //full extended
            if (flaps_left_inboard >= 100 && !FSIcm::inst->get<bool>(FSIID::MALFX_LE_FLAP_EXTENDED_LIGHT_OFF_ACTIVE)) {
                LightController::set(FSIID::MBI_LED_FLAPS_1_FULL_EXT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_FLAPS_2_FULL_EXT_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_LED_FLAPS_1_FULL_EXT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_FLAPS_2_FULL_EXT_LIGHT, false);
            }
            LightController::ProcessWrites();
            //debug("LED Left Flap: " + std::to_string(flaps_left_inboard));
        }
        
        //flaps 3 und 4
        if (id == FSIID::FSI_LEADING_EDGE_FLAPS_RIGHT_INBOARD_PERCENT || id == FSIID::MALFX_LE_FLAP_EXTENDED_LIGHT_OFF_ACTIVE) {
            byte flaps_right_inboard = FSIcm::inst->get<byte>(FSIID::FSI_LEADING_EDGE_FLAPS_RIGHT_INBOARD_PERCENT);
            
            //full extended
            if (flaps_right_inboard >= 100 && !FSIcm::inst->get<bool>(FSIID::MALFX_LE_FLAP_EXTENDED_LIGHT_OFF_ACTIVE)) {
                LightController::set(FSIID::MBI_LED_FLAPS_3_FULL_EXT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_FLAPS_4_FULL_EXT_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_LED_FLAPS_3_FULL_EXT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_FLAPS_4_FULL_EXT_LIGHT, false);
            }
            LightController::ProcessWrites();
            //debug("LED Right Flap: " + std::to_string(flaps_right_inboard));
        }
        
        //slats 1 - 4
        if (id == FSIID::FSI_LEADING_EDGE_FLAPS_LEFT_OUTBOARD_PERCENT || id == FSIID::MALFX_LE_FLAP_EXTENDED_LIGHT_OFF_ACTIVE) {
            byte slats_left_outboard = FSIcm::inst->get<byte>(FSIID::FSI_LEADING_EDGE_FLAPS_LEFT_OUTBOARD_PERCENT);
            
            //full extended
            if (slats_left_outboard >= 100 && !FSIcm::inst->get<bool>(FSIID::MALFX_LE_FLAP_EXTENDED_LIGHT_OFF_ACTIVE)) {
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
            if (slats_left_outboard > 0 && slats_left_outboard < 100 && !FSIcm::inst->get<bool>(FSIID::MALFX_LE_FLAP_EXTENDED_LIGHT_OFF_ACTIVE)) {
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
            //debug("LED Left Slat: " + std::to_string(slats_left_outboard));
        }
        
        //slats 1 - 4
        if (id == FSIID::FSI_LEADING_EDGE_FLAPS_RIGHT_OUTBOARD_PERCENT || id == FSIID::MALFX_LE_FLAP_EXTENDED_LIGHT_OFF_ACTIVE) {
            byte slats_right_outboard = FSIcm::inst->get<byte>(FSIID::FSI_LEADING_EDGE_FLAPS_RIGHT_OUTBOARD_PERCENT);
            
            //full extended
            if (slats_right_outboard >= 100 && !FSIcm::inst->get<bool>(FSIID::MALFX_LE_FLAP_EXTENDED_LIGHT_OFF_ACTIVE)) {
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
            if (slats_right_outboard > 0 && slats_right_outboard < 100 && !FSIcm::inst->get<bool>(FSIID::MALFX_LE_FLAP_EXTENDED_LIGHT_OFF_ACTIVE)) {
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
            //debug("LED Right Slat: " + std::to_string(slats_right_outboard));
        }
	}
    
    void LED::timedFunction(double time) {
        static double add_time = 1;
        static byte left_out_last=0, left_in_last=0, right_in_last=0, right_out_last=0;
        
        //only every one second
        add_time -= time;
        if (add_time <= 0) {
            add_time = 1;
            
            byte left_out = FSIcm::inst->get<byte>(FSIID::FSI_LEADING_EDGE_FLAPS_LEFT_OUTBOARD_PERCENT);
            byte left_in = FSIcm::inst->get<byte>(FSIID::FSI_LEADING_EDGE_FLAPS_LEFT_INBOARD_PERCENT);
            byte right_in = FSIcm::inst->get<byte>(FSIID::FSI_LEADING_EDGE_FLAPS_RIGHT_INBOARD_PERCENT);
            byte right_out = FSIcm::inst->get<byte>(FSIID::FSI_LEADING_EDGE_FLAPS_RIGHT_OUTBOARD_PERCENT);
            
            //TODO: Transit leuchtet nur, wenn LE Devices sich bewegen ODER irgendwo in der Mitte (z.B. bei einem Failure) stehen bleiben
            // Momentan leuchten sie immer, wenn sich etwas bewegt. Ohne failures reicht das auch.
            //slats 1-4
            bool any_flap_trans = false;
            if (left_out != left_out_last || FSIcm::inst->get<bool>(FSIID::MALFX_LE_FLAP_TRANSIT_LIGHT_ON_ACTIVE)) {
                LightController::set(FSIID::MBI_LED_SLATS_1_TRANSIT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_2_TRANSIT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_3_TRANSIT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_4_TRANSIT_LIGHT, true);
                any_flap_trans = true;
            } else {
                LightController::set(FSIID::MBI_LED_SLATS_1_TRANSIT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_2_TRANSIT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_3_TRANSIT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_4_TRANSIT_LIGHT, false);
            }
            
            //flaps 1-2
            if (left_in != left_in_last || FSIcm::inst->get<bool>(FSIID::MALFX_LE_FLAP_TRANSIT_LIGHT_ON_ACTIVE)) {
                LightController::set(FSIID::MBI_LED_FLAPS_1_TRANSIT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_FLAPS_2_TRANSIT_LIGHT, true);
                any_flap_trans = true;
            } else {
                LightController::set(FSIID::MBI_LED_FLAPS_1_TRANSIT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_FLAPS_2_TRANSIT_LIGHT, false);
            }
            
            //flaps 3-4
            if (right_in != right_in_last || FSIcm::inst->get<bool>(FSIID::MALFX_LE_FLAP_TRANSIT_LIGHT_ON_ACTIVE)) {
                LightController::set(FSIID::MBI_LED_FLAPS_3_TRANSIT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_FLAPS_4_TRANSIT_LIGHT, true);
                any_flap_trans = true;
            } else {
                LightController::set(FSIID::MBI_LED_FLAPS_3_TRANSIT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_FLAPS_4_TRANSIT_LIGHT, false);
            }
            
            //slats 5-8
            if (right_out != right_out_last || FSIcm::inst->get<bool>(FSIID::MALFX_LE_FLAP_TRANSIT_LIGHT_ON_ACTIVE)) {
                LightController::set(FSIID::MBI_LED_SLATS_5_TRANSIT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_6_TRANSIT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_7_TRANSIT_LIGHT, true);
                LightController::set(FSIID::MBI_LED_SLATS_8_TRANSIT_LIGHT, true);
                any_flap_trans = true;
            } else {
                LightController::set(FSIID::MBI_LED_SLATS_5_TRANSIT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_6_TRANSIT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_7_TRANSIT_LIGHT, false);
                LightController::set(FSIID::MBI_LED_SLATS_8_TRANSIT_LIGHT, false);
            }
            
            
            
            left_out_last = left_out;
            left_in_last = left_in;
            right_in_last = right_in;
            right_out_last = right_out;


            //MIP Lights
            if (any_flap_trans || FSIcm::inst->get<bool>(FSIID::MALFX_LE_FLAP_TRANSIT_LIGHT_ON_ACTIVE)) {
                LightController::set(FSIID::MBI_MIP_CENTER_LE_FLAPS_TRANSIT_LIGHT, true);
                LightController::set(FSIID::MBI_MIP_CENTER_LE_FLAPS_EXT_LIGHT, false);
            }
            else {
                LightController::set(FSIID::MBI_MIP_CENTER_LE_FLAPS_TRANSIT_LIGHT, false);
                if ((left_out > 0 || left_in > 0 || right_in > 0 || right_out > 0) && !FSIcm::inst->get<bool>(FSIID::MALFX_LE_FLAP_EXTENDED_LIGHT_OFF_ACTIVE)) {
                    LightController::set(FSIID::MBI_MIP_CENTER_LE_FLAPS_EXT_LIGHT, true);
                } else {
                    LightController::set(FSIID::MBI_MIP_CENTER_LE_FLAPS_EXT_LIGHT, false);
                }
            }

            LightController::ProcessWrites();
        }
    }
}