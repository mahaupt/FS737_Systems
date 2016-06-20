#include "FUEL.h"

namespace fssystems
{
	FUEL * FUEL::instance = nullptr;

	FUEL::FUEL()
	{
		//debug variable
		is_debug = true;

		instance = this;

		//starting FSI Client for FUEL
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);
		FSIID wanted_vars[] =
		{
			FSIID::MBI_FUEL_CROSSFEED_SWITCH,
			FSIID::MBI_FUEL_CTR_LEFT_PUMP_SWITCH,
			FSIID::MBI_FUEL_CTR_RIGHT_PUMP_SWITCH,
			FSIID::MBI_FUEL_LEFT_AFT_PUMP_SWITCH,
			FSIID::MBI_FUEL_LEFT_FWD_PUMP_SWITCH,
			FSIID::MBI_FUEL_RIGHT_AFT_PUMP_SWITCH,
			FSIID::MBI_FUEL_RIGHT_FWD_PUMP_SWITCH,
			FSIID::SLI_FUEL_CENTRE_CAPACITY,
			FSIID::SLI_FUEL_LEFT_MAIN_CAPACITY,
			FSIID::SLI_FUEL_RIGHT_MAIN_CAPACITY
		};
		FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));

		//standard values
		LightController::registerLight(FSIID::MBI_FUEL_CROSSFEED_VALVE_OPEN_LIGHT, FSIID::MBI_FUEL_CROSSFEED_VALVE_OPEN_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_FUEL_CTR_LEFT_PUMP_LOW_PRESSURE_LIGHT);
		LightController::registerLight(FSIID::MBI_FUEL_CTR_RIGHT_PUMP_LOW_PRESSURE_LIGHT);
		LightController::registerLight(FSIID::MBI_FUEL_LEFT_AFT_PUMP_LOW_PRESSURE_LIGHT);
		LightController::registerLight(FSIID::MBI_FUEL_LEFT_ENG_VALVE_CLOSED_LIGHT, FSIID::MBI_FUEL_LEFT_ENG_VALVE_CLOSED_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_FUEL_LEFT_FILTER_BYPASS_LIGHT);
		LightController::registerLight(FSIID::MBI_FUEL_LEFT_FWD_PUMP_LOW_PRESSURE_LIGHT);
		LightController::registerLight(FSIID::MBI_FUEL_LEFT_SPAR_VALVE_CLOSED_LIGHT, FSIID::MBI_FUEL_LEFT_SPAR_VALVE_CLOSED_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_FUEL_RIGHT_AFT_PUMP_LOW_PRESSURE_LIGHT);
		LightController::registerLight(FSIID::MBI_FUEL_RIGHT_ENG_VALVE_CLOSED_LIGHT, FSIID::MBI_FUEL_RIGHT_ENG_VALVE_CLOSED_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_FUEL_RIGHT_FILTER_BYPASS_LIGHT);
		LightController::registerLight(FSIID::MBI_FUEL_RIGHT_FWD_PUMP_LOW_PRESSURE_LIGHT);
		LightController::registerLight(FSIID::MBI_FUEL_RIGHT_SPAR_VALVE_CLOSED_LIGHT, FSIID::MBI_FUEL_RIGHT_SPAR_VALVE_CLOSED_LIGHT_DIMMED);

		FSIcm::inst->set<bool>(FSIID::MBI_FUEL_LAMPTEST, false);

		FSIcm::inst->ProcessWrites();
	}

	void FUEL::fsiOnVarReceive(FSIID id) {
		instance->onVarReceive(id);
	}

	void FUEL::run_all_machines()
	{
		
	}

	void FUEL::onVarReceive(FSIID & id)
	{

		static bool always_power = true, never_fail = false;  // Static Values to make statemachine-functions easier to read

		//CROSSFEED
		if (id == FSIID::MBI_FUEL_CROSSFEED_SWITCH)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_FUEL_CROSSFEED_SWITCH))
			{
				debug("FUEL Crossfeed On");
				valve_xfeed.run_machine(true);
			}
			else
			{
				debug("FUEL Crossfeed Off");
				valve_xfeed.run_machine(false);
			}

			//ELT light
			LightController::set(FSIID::MBI_FUEL_CROSSFEED_VALVE_OPEN_LIGHT, FSIcm::inst->get<bool>(FSIID::MBI_FUEL_CROSSFEED_SWITCH));
			LightController::ProcessWrites();
		}

		//FUEL CTR L
		if (id == FSIID::MBI_FUEL_CTR_LEFT_PUMP_SWITCH)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_FUEL_CTR_LEFT_PUMP_SWITCH))
			{
				debug("FUEL CTR LEFT PUMP On");
			}
			else
			{
				debug("FUEL CTR LEFT PUMP Off");
			}

			center_left.run_machine(FSIcm::inst->get<bool>(FSIID::MBI_FUEL_CTR_LEFT_PUMP_SWITCH), always_power, never_fail);

			//ELT light
			LightController::set(FSIID::MBI_FUEL_CTR_LEFT_PUMP_LOW_PRESSURE_LIGHT, !FSIcm::inst->get<bool>(FSIID::MBI_FUEL_CTR_LEFT_PUMP_SWITCH));
			LightController::ProcessWrites();
		}

		//FUEL CTR R
		if (id == FSIID::MBI_FUEL_CTR_RIGHT_PUMP_SWITCH)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_FUEL_CTR_RIGHT_PUMP_SWITCH))
			{
				debug("FUEL CTR RIGHT PUMP On");
			}
			else
			{
				debug("FUEL CTR RIGHT PUMP Off");
			}

			center_right.run_machine(FSIcm::inst->get<bool>(FSIID::MBI_FUEL_CTR_RIGHT_PUMP_SWITCH), always_power, never_fail);

			//ELT light
			LightController::set(FSIID::MBI_FUEL_CTR_RIGHT_PUMP_LOW_PRESSURE_LIGHT, !FSIcm::inst->get<bool>(FSIID::MBI_FUEL_CTR_RIGHT_PUMP_SWITCH));
			LightController::ProcessWrites();
		}


		//FUEL AFT L
		if (id == FSIID::MBI_FUEL_LEFT_AFT_PUMP_SWITCH)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_FUEL_LEFT_AFT_PUMP_SWITCH))
			{
				debug("FUEL AFT LEFT PUMP On");
			}
			else
			{
				debug("FUEL AFT LEFT PUMP Off");
			}

			left_aft.run_machine(FSIcm::inst->get<bool>(FSIID::MBI_FUEL_LEFT_AFT_PUMP_SWITCH), always_power, never_fail);

			//ELT light
			LightController::set(FSIID::MBI_FUEL_LEFT_AFT_PUMP_LOW_PRESSURE_LIGHT, !FSIcm::inst->get<bool>(FSIID::MBI_FUEL_LEFT_AFT_PUMP_SWITCH));
			LightController::ProcessWrites();
		}

		//FUEL AFT R
		if (id == FSIID::MBI_FUEL_RIGHT_AFT_PUMP_SWITCH)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_FUEL_RIGHT_AFT_PUMP_SWITCH))
			{
				debug("FUEL AFT RIGHT PUMP On");
			}
			else
			{
				debug("FUEL AFT RIGHT PUMP Off");
			}

			right_aft.run_machine(FSIcm::inst->get<bool>(FSIID::MBI_FUEL_RIGHT_AFT_PUMP_SWITCH), always_power, never_fail);

			//ELT light
			LightController::set(FSIID::MBI_FUEL_RIGHT_AFT_PUMP_LOW_PRESSURE_LIGHT, !FSIcm::inst->get<bool>(FSIID::MBI_FUEL_RIGHT_AFT_PUMP_SWITCH));
			LightController::ProcessWrites();
		}

		//FUEL FWD L
		if (id == FSIID::MBI_FUEL_LEFT_FWD_PUMP_SWITCH)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_FUEL_LEFT_FWD_PUMP_SWITCH))
			{
				debug("FUEL FWD LEFT PUMP On");
			}
			else
			{
				debug("FUEL FWD LEFT PUMP Off");
			}

			left_forward.run_machine(FSIcm::inst->get<bool>(FSIID::MBI_FUEL_LEFT_FWD_PUMP_SWITCH), always_power, never_fail);

			//ELT light
			LightController::set(FSIID::MBI_FUEL_LEFT_FWD_PUMP_LOW_PRESSURE_LIGHT, !FSIcm::inst->get<bool>(FSIID::MBI_FUEL_LEFT_FWD_PUMP_SWITCH));
			LightController::ProcessWrites();
		}

		//FUEL FWD R
		if (id == FSIID::MBI_FUEL_RIGHT_FWD_PUMP_SWITCH)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_FUEL_RIGHT_FWD_PUMP_SWITCH))
			{
				debug("FUEL FWD RIGHT PUMP On");
			}
			else
			{
				debug("FUEL FWD RIGHT PUMP Off");
			}

			right_forward.run_machine(FSIcm::inst->get<bool>(FSIID::MBI_FUEL_RIGHT_FWD_PUMP_SWITCH), always_power, never_fail);

			//ELT light
			LightController::set(FSIID::MBI_FUEL_RIGHT_FWD_PUMP_LOW_PRESSURE_LIGHT, !FSIcm::inst->get<bool>(FSIID::MBI_FUEL_RIGHT_FWD_PUMP_SWITCH));
			LightController::ProcessWrites();
		}
		
		pumpstate leftstates[] = { left_aft.actual_state, left_forward.actual_state, center_left.actual_state };
		pumpstate rightstates[] = { right_aft.actual_state, right_forward.actual_state, center_right.actual_state };
		pipeline_left.run_machine(leftstates, rightstates, valve_xfeed.actual_state, sOPEN);
		pipeline_right.run_machine(rightstates, leftstates, valve_xfeed.actual_state, sOPEN);

		if (!pipeline_left.actual_state == sSHUTOFF) //ENG 1 SHUTOFF VALVES NOT CLOSED
		{
			if (pipeline_left.actual_state == sUNPRESSURIZED && FSIcm::inst->get<bool>(FSIID::SLI_FUEL_LEFT_MAIN_CAPACITY) && (FSIcm::inst->get<double>(FSIID::SLI_ENG1_N1) > 50)) // SUCTION FEED POSSIBLE
			{
				FSIcm::inst->set(FSIID::SLI_FUEL_ENG1_AVAIL, true);
			}
			if (pipeline_left.actual_state == sPRESSURIZED) // PUPS ACITVE AND DELIVERING FUEL PRESSURE
			{
				FSIcm::inst->set(FSIID::SLI_FUEL_ENG1_AVAIL, true);
			}
			else FSIcm::inst->set(FSIID::SLI_FUEL_ENG1_AVAIL, false);
		}
		else FSIcm::inst->set(FSIID::SLI_FUEL_ENG1_AVAIL, false);
	}
}