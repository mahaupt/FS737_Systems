#include "FRECSTALLTEST.h"


namespace fssystems
{
	FRECSTALLTEST * FRECSTALLTEST::instance = nullptr;

	FRECSTALLTEST::FRECSTALLTEST()
	{
		//debug variable
		is_debug = true;

		instance = this;

		//starting FSI Client for IRS
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);
		FSIID wanted_var[] =
		{
			FSIID::MBI_FLIGHT_REC_SPEED_WARNING_TEST_1_SWITCH,
			FSIID::MBI_FLIGHT_REC_SPEED_WARNING_TEST_2_SWITCH,
			FSIID::MBI_FLIGHT_REC_STALL_WARNING_TEST_1_SWITCH,
			FSIID::MBI_FLIGHT_REC_STALL_WARNING_TEST_2_SWITCH,
			FSIID::MBI_FLIGHT_REC_TEST_SWITCH
		};
		FSIcm::inst->DeclareAsWanted(wanted_var, sizeof(wanted_var));

		//standard values
		LightController::registerLight(FSIID::MBI_FLIGHT_REC_OFF_LIGHT);

		FSIcm::inst->set<bool>(FSIID::MBI_FLIGHT_REC_LAMPTEST, false);

		FSIcm::inst->ProcessWrites();
	}


	void FRECSTALLTEST::fsiOnVarReceive(FSIID id) {
		instance->onVarReceive(id);
	}


	void FRECSTALLTEST::onVarReceive(FSIID & id)
	{
		//FLIGHT REC TEST
		if (id == FSIID::MBI_FLIGHT_REC_TEST_SWITCH)
		{
			if (!FSIcm::inst->get<bool>(FSIID::MBI_FLIGHT_REC_TEST_SWITCH))
			{
				debug("FLIGHT REC TEST");
				LightController::set(FSIID::MBI_FLIGHT_REC_OFF_LIGHT, false);
			}
			else
			{
				debug("FLIGHT REC NORM");
				LightController::set(FSIID::MBI_FLIGHT_REC_OFF_LIGHT, true);
			}
			LightController::ProcessWrites();
		}
	}


}