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
			FSIID::MBI_LED_TEST_SWITCH
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
	}
}