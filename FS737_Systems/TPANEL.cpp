#include "TPANEL.h"

namespace fssystems
{
	TPANEL * TPANEL::instance = nullptr;

	TPANEL::TPANEL()
	{
		//debug variable
		is_debug = true;

		instance = this;

		//starting FSI Client for IRS
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);
		FSIID wanted_vars[] =
		{
			FSIID::MBI_LOWER_T_MIDDLE_EMER_EXIT_LIGHTS_SWITCH_OFF_POS,
			FSIID::MBI_LOWER_T_MIDDLE_EMER_EXIT_LIGHTS_SWITCH_ON_POS,

			FSIID::FSI_GEAR_ACTUAL_NOSE,
			FSIID::FSI_GEAR_ACTUAL_RIGHT,
			FSIID::FSI_GEAR_ACTUAL_LEFT,

			//lights
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_ANTI_COLLISION_SWITCH,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_LANDING_FIXED_LEFT_SWITCH,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_LANDING_FIXED_RIGHT_SWITCH,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_LANDING_RETRACTABLE_LEFT_SWITCH_ON_POS,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_LANDING_RETRACTABLE_LEFT_SWITCH_RETRACT_POS,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_LANDING_RETRACTABLE_RIGHT_SWITCH_ON_POS,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_LANDING_RETRACTABLE_RIGHT_SWITCH_RETRACT_POS,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_LOGO_SWITCH,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_POSITION_SWITCH_ON_BAT_POS,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_POSITION_SWITCH_ON_POS,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_RUNWAY_TURNOFF_LEFT_SWITCH,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_RUNWAY_TURNOFF_RIGHT_SWITCH,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_STROBE_SWITCH_OFF_POS,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_STROBE_SWITCH_ON_POS,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_TAXI_SWITCH_AUTO_BRT_POS,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_TAXI_SWITCH_OFF_POS,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_WHEEL_WELL_SWITCH,
			FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_WING_SWITCH
		};
		FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));

		//standard values
		LightController::registerLight(FSIID::MBI_LOWER_T_LAVATORY_SMOKE_LIGHT);
		LightController::registerLight(FSIID::MBI_LOWER_T_MIDDLE_EQUIP_COOLING_EXHAUST_LIGHT);
		LightController::registerLight(FSIID::MBI_LOWER_T_MIDDLE_EQUIP_COOLING_SUPPLY_LIGHT);
		LightController::registerLight(FSIID::MBI_LOWER_T_MIDDLE_ATTEND_CALL_LIGHT);
		LightController::registerLight(FSIID::MBI_LOWER_T_MIDDLE_EMER_EXIT_LIGHTS_NOT_ARMED_LIGHT);

		LightController::registerLight(FSIID::MBI_UPPER_T_LEFT_GEAR_LIGHT, FSIID::MBI_UPPER_T_LEFT_GEAR_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_UPPER_T_NOSE_GEAR_LIGHT, FSIID::MBI_UPPER_T_NOSE_GEAR_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_UPPER_T_RIGHT_GEAR_LIGHT, FSIID::MBI_UPPER_T_RIGHT_GEAR_LIGHT_DIMMED);

		LightController::registerLight(FSIID::MBI_UPPER_T_PSEU_LIGHT, FSIID::MBI_UPPER_T_PSEU_LIGHT_DIMMED);

		FSIcm::inst->set<bool>(FSIID::MBI_LOWER_T_LAMPTEST, false);
		FSIcm::inst->set<bool>(FSIID::MBI_UPPER_T_LAMPTEST, false);

		FSIcm::inst->ProcessWrites();
	}

	void TPANEL::fsiOnVarReceive(FSIID id)
	{
		instance->onVarReceive(id);
	}

	void TPANEL::onVarReceive(FSIID & id)
	{
		//EMER EXIT LIGHTS
		if (id == FSIID::MBI_LOWER_T_MIDDLE_EMER_EXIT_LIGHTS_SWITCH_OFF_POS)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_MIDDLE_EMER_EXIT_LIGHTS_SWITCH_ON_POS))
			{
				debug("EMER EXIT LIGHTS ON");
				LightController::set(FSIID::MBI_LOWER_T_MIDDLE_EMER_EXIT_LIGHTS_NOT_ARMED_LIGHT, false);
			}
			else if (FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_MIDDLE_EMER_EXIT_LIGHTS_SWITCH_OFF_POS))
			{
				debug("EMER EXIT LIGHTS OFF");
				LightController::set(FSIID::MBI_LOWER_T_MIDDLE_EMER_EXIT_LIGHTS_NOT_ARMED_LIGHT, true);
			}
			else
			{
				debug("EMER EXIT LIGHTS ARM");
				LightController::set(FSIID::MBI_LOWER_T_MIDDLE_EMER_EXIT_LIGHTS_NOT_ARMED_LIGHT, false);
			}

			LightController::ProcessWrites();
		}

		//GEAR LIGHTS
		if (id == FSIID::FSI_GEAR_ACTUAL_NOSE)
		{
			if (FSIcm::inst->get<long>(FSIID::FSI_GEAR_ACTUAL_NOSE) >= 16383)
			{
				LightController::set(FSIID::MBI_UPPER_T_NOSE_GEAR_LIGHT, true);
			}
			else
			{
				LightController::set(FSIID::MBI_UPPER_T_NOSE_GEAR_LIGHT, false);
			}
			LightController::ProcessWrites();
		}
		if (id == FSIID::FSI_GEAR_ACTUAL_LEFT)
		{
			if (FSIcm::inst->get<long>(FSIID::FSI_GEAR_ACTUAL_LEFT) >= 16383)
			{
				LightController::set(FSIID::MBI_UPPER_T_LEFT_GEAR_LIGHT, true);
			}
			else
			{
				LightController::set(FSIID::MBI_UPPER_T_LEFT_GEAR_LIGHT, false);
			}
			LightController::ProcessWrites();
		}
		if (id == FSIID::FSI_GEAR_ACTUAL_RIGHT)
		{
			if (FSIcm::inst->get<long>(FSIID::FSI_GEAR_ACTUAL_RIGHT) >= 16383)
			{
				LightController::set(FSIID::MBI_UPPER_T_RIGHT_GEAR_LIGHT, true);
			}
			else
			{
				LightController::set(FSIID::MBI_UPPER_T_RIGHT_GEAR_LIGHT, false);
			}
			LightController::ProcessWrites();
		}



		//LIGHTS##################################
		//beacon / anti-collision
		if (id == FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_ANTI_COLLISION_SWITCH)
		{
			FSIcm::inst->set<bool>(FSIID::FSI_LIGHTS_BEACON, FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_ANTI_COLLISION_SWITCH));
			debug("LOWER_T Beacon " + FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_ANTI_COLLISION_SWITCH));
			FSIcm::inst->ProcessWrites();
		}

		//position / nav lights
		if (id == FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_POSITION_SWITCH_ON_POS)
		{
			FSIcm::inst->set<bool>(FSIID::FSI_LIGHTS_NAV, FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_POSITION_SWITCH_ON_POS));
			debug("LOWER_T Navlights " + FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_POSITION_SWITCH_ON_POS));
			FSIcm::inst->ProcessWrites();
		}

		//strobe lights
		if (id == FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_STROBE_SWITCH_ON_POS)
		{
			FSIcm::inst->set<bool>(FSIID::FSI_LIGHTS_STROBE, FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_STROBE_SWITCH_ON_POS));
			debug("LOWER_T Strobes " + FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_STROBE_SWITCH_ON_POS));
			FSIcm::inst->ProcessWrites();
		}

		//taxi lights
		if (id == FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_TAXI_SWITCH_AUTO_BRT_POS)
		{
			FSIcm::inst->set<bool>(FSIID::FSI_LIGHTS_TAXI, FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_TAXI_SWITCH_AUTO_BRT_POS));
			debug("LOWER_T Taxi " + FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_TAXI_SWITCH_AUTO_BRT_POS));
			FSIcm::inst->ProcessWrites();
		}

		//landing lights
		if (id == FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_LANDING_FIXED_LEFT_SWITCH)
		{
			FSIcm::inst->set<bool>(FSIID::FSI_LIGHTS_LANDING, FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_LANDING_FIXED_LEFT_SWITCH));
			debug("LOWER_T Landing " + FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_LANDING_FIXED_LEFT_SWITCH));
			FSIcm::inst->ProcessWrites();
		}

		//logo lights
		if (id == FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_LOGO_SWITCH)
		{
			FSIcm::inst->set<bool>(FSIID::FSI_LIGHTS_LOGO, FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_LOGO_SWITCH));
			debug("LOWER_T Logo " + FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_LOGO_SWITCH));
			FSIcm::inst->ProcessWrites();
		}

		//wing lights
		if (id == FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_WING_SWITCH)
		{
			FSIcm::inst->set<bool>(FSIID::FSI_LIGHTS_WING, FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_WING_SWITCH));
			debug("LOWER_T Wing " + FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_BOTTOM_LIGHTS_WING_SWITCH));
			FSIcm::inst->ProcessWrites();
		}
	}
}