#include "AIRCOND.h"

namespace fssystems
{

	AIRCOND::AIRCOND()
	{
		//debug variable
		is_debug = true;

		//starting FSI Client for IRS
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);

		//standard values
		LightController::registerLight(FSIID::MBI_AIR_COND_CONT_CAB_ZONE_TEMP_LIGHT);
		LightController::registerLight(FSIID::MBI_AIR_COND_AFT_CAB_ZONE_TEMP_LIGHT);
		LightController::registerLight(FSIID::MBI_AIR_COND_FWD_CAB_ZONE_TEMP_LIGHT);

		FSIcm::inst->set<bool>(FSIID::MBI_AIR_COND_LAMPTEST, false);

		FSIcm::inst->ProcessWrites();
	}

}