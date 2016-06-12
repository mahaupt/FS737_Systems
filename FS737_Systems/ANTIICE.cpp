#include "ANTIICE.h"


namespace fssystems
{
	ANTIICE * ANTIICE::instance = nullptr;

	ANTIICE::ANTIICE()
	{
		instance = this;

		//debug variable
		is_debug = true;

		//starting FSI Client for IRS
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);
		FSIID wanted_vars[] =
		{
			FSIID::MBI_ANTI_ICE_ENG_1_ANTI_ICE_SWITCH,
			FSIID::MBI_ANTI_ICE_ENG_2_ANTI_ICE_SWITCH,
			FSIID::MBI_ANTI_ICE_WING_ANTI_ICE_SWITCH
		};
		FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));

		//standard values
		LightController::registerLight(FSIID::MBI_ANTI_ICE_WING_L_VALVE_OPEN_LIGHT, FSIID::MBI_ANTI_ICE_WING_L_VALVE_OPEN_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_ANTI_ICE_WING_R_VALVE_OPEN_LIGHT, FSIID::MBI_ANTI_ICE_WING_R_VALVE_OPEN_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_ANTI_ICE_ENG_2_COWL_VALVE_OPEN_LIGHT, FSIID::MBI_ANTI_ICE_ENG_2_COWL_VALVE_OPEN_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_ANTI_ICE_ENG_1_COWL_VALVE_OPEN_LIGHT, FSIID::MBI_ANTI_ICE_ENG_1_COWL_VALVE_OPEN_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_ANTI_ICE_ENG_2_COWL_ANTI_ICE_LIGHT);
		LightController::registerLight(FSIID::MBI_ANTI_ICE_ENG_1_COWL_ANTI_ICE_LIGHT);

		FSIcm::inst->set<bool>(FSIID::MBI_ANTI_ICE_LAMPTEST, false);

		FSIcm::inst->ProcessWrites();
	}

	void ANTIICE::fsiOnVarReceive(FSIID id)
	{
		instance->onVarReceive(id);
	}

	void ANTIICE::onVarReceive(FSIID & id)
	{

		//WING
		if (id == FSIID::MBI_ANTI_ICE_WING_ANTI_ICE_SWITCH && FSIcm::inst->get<bool>(FSIID::MBI_ANTI_ICE_WING_ANTI_ICE_SWITCH) == false)
		{
			debug("ANTI_ICE Wing On");

			LightController::set(FSIID::MBI_ANTI_ICE_WING_L_VALVE_OPEN_LIGHT, true);
			LightController::set(FSIID::MBI_ANTI_ICE_WING_R_VALVE_OPEN_LIGHT, true);
			LightController::ProcessWrites();
		}
		if (id == FSIID::MBI_ANTI_ICE_WING_ANTI_ICE_SWITCH && FSIcm::inst->get<bool>(FSIID::MBI_ANTI_ICE_WING_ANTI_ICE_SWITCH) == true)
		{
			debug("ANTI_ICE Wing Off");

			LightController::set(FSIID::MBI_ANTI_ICE_WING_L_VALVE_OPEN_LIGHT, false);
			LightController::set(FSIID::MBI_ANTI_ICE_WING_R_VALVE_OPEN_LIGHT, false);
			LightController::ProcessWrites();
		}


		//ENG 1
		if (id == FSIID::MBI_ANTI_ICE_ENG_1_ANTI_ICE_SWITCH && FSIcm::inst->get<bool>(FSIID::MBI_ANTI_ICE_ENG_1_ANTI_ICE_SWITCH) == false)
		{
			debug("ANTI_ICE ENG 1 On");

			LightController::set(FSIID::MBI_ANTI_ICE_ENG_1_COWL_VALVE_OPEN_LIGHT, true);
			LightController::ProcessWrites();
		}
		if (id == FSIID::MBI_ANTI_ICE_ENG_1_ANTI_ICE_SWITCH && FSIcm::inst->get<bool>(FSIID::MBI_ANTI_ICE_ENG_1_ANTI_ICE_SWITCH) == true)
		{
			debug("ANTI_ICE ENG 1 Off");

			LightController::set(FSIID::MBI_ANTI_ICE_ENG_1_COWL_VALVE_OPEN_LIGHT, false);
			LightController::ProcessWrites();
		}


		//ENG 2
		if (id == FSIID::MBI_ANTI_ICE_ENG_2_ANTI_ICE_SWITCH && FSIcm::inst->get<bool>(FSIID::MBI_ANTI_ICE_ENG_2_ANTI_ICE_SWITCH) == false)
		{
			debug("ANTI_ICE ENG 2 On");

			LightController::set(FSIID::MBI_ANTI_ICE_ENG_2_COWL_VALVE_OPEN_LIGHT, true);
			LightController::ProcessWrites();
		}
		if (id == FSIID::MBI_ANTI_ICE_ENG_2_ANTI_ICE_SWITCH && FSIcm::inst->get<bool>(FSIID::MBI_ANTI_ICE_ENG_2_ANTI_ICE_SWITCH) == true)
		{
			debug("ANTI_ICE ENG 2 Off");

			LightController::set(FSIID::MBI_ANTI_ICE_ENG_2_COWL_VALVE_OPEN_LIGHT, false);
			LightController::ProcessWrites();
		}
	}

}

