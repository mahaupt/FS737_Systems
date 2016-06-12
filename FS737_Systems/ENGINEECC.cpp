#include "ENGINEECC.h"

namespace fssystems {



	ENGINEECC::ENGINEECC()
	{
		//debug variable
		is_debug = true;

		//starting FSI Client for IRS
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);
		FSIID wanted_vars[] =
		{
			FSIID::FSI_ENG1_THROTTLE_LEVER,
			FSIID::FSI_ENG2_THROTTLE_LEVER
		};
		FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));

		//standard values
		LightController::registerLight(FSIID::MBI_EEC_ENG_1_ENGINE_CONTROL_LIGHT);
		LightController::registerLight(FSIID::MBI_EEC_ENG_1_EEC_ALTN_LIGHT);
		LightController::registerLight(FSIID::MBI_EEC_ENG_1_EEC_ON_LIGHT);
		LightController::registerLight(FSIID::MBI_EEC_ENG_1_REVERSER_LIGHT);

		LightController::registerLight(FSIID::MBI_EEC_ENG_2_ENGINE_CONTROL_LIGHT);
		LightController::registerLight(FSIID::MBI_EEC_ENG_2_EEC_ALTN_LIGHT);
		LightController::registerLight(FSIID::MBI_EEC_ENG_2_EEC_ON_LIGHT);
		LightController::registerLight(FSIID::MBI_EEC_ENG_2_REVERSER_LIGHT);

		FSIcm::inst->set<bool>(FSIID::MBI_EEC_LAMPTEST, false);

		FSIcm::inst->ProcessWrites();
	}



	void ENGINEECC::fsiOnVarReceive(FSIID id)
	{
		if (id == FSIID::FSI_ENG1_THROTTLE_LEVER)
		{
			if (FSIcm::inst->get<float>(FSIID::FSI_ENG1_THROTTLE_LEVER) < 0)
			{
				LightController::set(FSIID::MBI_EEC_ENG_1_REVERSER_LIGHT, true);
			}
			else
			{
				LightController::set(FSIID::MBI_EEC_ENG_1_REVERSER_LIGHT, false);
			}
			LightController::ProcessWrites();
		}

		if (id == FSIID::FSI_ENG2_THROTTLE_LEVER)
		{
			if (FSIcm::inst->get<float>(FSIID::FSI_ENG2_THROTTLE_LEVER) < 0)
			{
				LightController::set(FSIID::MBI_EEC_ENG_2_REVERSER_LIGHT, true);
			}
			else
			{
				LightController::set(FSIID::MBI_EEC_ENG_2_REVERSER_LIGHT, false);
			}
			LightController::ProcessWrites();
		}
	}



}