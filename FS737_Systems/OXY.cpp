#include "OXY.h"

namespace fssystems
{
	OXY * OXY::instance = nullptr;

	OXY::OXY()
	{
		//debug variable
		is_debug = true;

		instance = this;

		//starting FSI Client for IRS
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);
		FSIID wanted_vars[] =
		{
			FSIID::MBI_OXYGEN_PASS_OXY_SWITCH
		};
		FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));

		//standard values
		LightController::registerLight(FSIID::MBI_OXYGEN_PASS_OXY_LIGHT, FSIID::MBI_OXYGEN_PASS_OXY_LIGHT_DIMMED);

		FSIcm::inst->set<bool>(FSIID::MBI_OXYGEN_LAMPTEST, false);

		FSIcm::inst->ProcessWrites();
	}

	void OXY::fsiOnVarReceive(FSIID id) {
		instance->onVarReceive(id);
	}

	void OXY::onVarReceive(FSIID & id)
	{
		if (id == FSIID::MBI_OXYGEN_PASS_OXY_SWITCH)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_OXYGEN_PASS_OXY_SWITCH) == true)
			{
				debug("OXY Pass ON");
			}
			else
			{
				debug("OXY Pass NORM");
			}

			//ELT light
			LightController::set(FSIID::MBI_OXYGEN_PASS_OXY_LIGHT, FSIcm::inst->get<bool>(FSIID::MBI_OXYGEN_PASS_OXY_SWITCH));
			LightController::ProcessWrites();
		}
	}


}