#include "ELT.h"

namespace fssystems
{
	ELT * ELT::instance = nullptr;

	ELT::ELT()
	{
		instance = this;

		//debug variable
		is_debug = true;

		//starting FSI Client for IRS
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);

		FSIID wanted_vars[] =
		{
			FSIID::MBI_ELT_ARM_SWITCH
		};
		FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));

		//standard values
		LightController::registerLight(FSIID::MBI_ELT_ACTIVE_LIGHT);

		FSIcm::inst->set<bool>(FSIID::MBI_ELT_LAMPTEST, false);
		FSIcm::inst->ProcessWrites();
	}


	void ELT::fsiOnVarReceive(FSIID id)
	{
		if (id == FSIID::MBI_ELT_ARM_SWITCH)
		{
			if (!FSIcm::inst->get<bool>(FSIID::MBI_ELT_ARM_SWITCH))
			{
				instance->debug("ELT On");
			}
			else
			{
				instance->debug("ELT Arm");
			}


			//ELT light
			LightController::set(FSIID::MBI_ELT_ACTIVE_LIGHT, !FSIcm::inst->get<bool>(FSIID::MBI_ELT_ARM_SWITCH));
			LightController::ProcessWrites();
		}
	}
}