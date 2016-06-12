#include "FUEL.h"

namespace fssystems
{
	FUEL * FUEL::instance = nullptr;

	FUEL::FUEL()
	{
		//debug variable
		is_debug = true;

		instance = this;

		//starting FSI Client for IRS
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);
		FSIID wanted_vars[] =
		{
			FSIID::MBI_FUEL_CROSSFEED_SWITCH,
			FSIID::MBI_FUEL_CTR_LEFT_PUMP_SWITCH,
			FSIID::MBI_FUEL_CTR_RIGHT_PUMP_SWITCH,
			FSIID::MBI_FUEL_LEFT_AFT_PUMP_SWITCH,
			FSIID::MBI_FUEL_LEFT_FWD_PUMP_SWITCH,
			FSIID::MBI_FUEL_RIGHT_AFT_PUMP_SWITCH,
			FSIID::MBI_FUEL_RIGHT_FWD_PUMP_SWITCH
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

	void FUEL::onVarReceive(FSIID & id)
	{
		//CROSSFEED
		if (id == FSIID::MBI_FUEL_CROSSFEED_SWITCH)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_FUEL_CROSSFEED_SWITCH))
			{
				debug("FUEL Crossfeed On");
			}
			else
			{
				debug("FUEL Crossfeed Off");
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

			//ELT light
			LightController::set(FSIID::MBI_FUEL_RIGHT_FWD_PUMP_LOW_PRESSURE_LIGHT, !FSIcm::inst->get<bool>(FSIID::MBI_FUEL_RIGHT_FWD_PUMP_SWITCH));
			LightController::ProcessWrites();
		}
	}

}