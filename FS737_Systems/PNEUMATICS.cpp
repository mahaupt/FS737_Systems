#include "PNEUMATICS.h"

namespace fssystems
{

	PNEUMATICS::PNEUMATICS()
	{
		//debug variable
		is_debug = true;

		//starting FSI Client for IRS
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);
		/*FSIcm::inst->DeclareAsWanted(new FSIID[]
		{
		FSIID::MBI_ELT_ARM_SWITCH
		}
		);*/

		//standard values
		LightController::registerLight(FSIID::MBI_PNEUMATICS_ALTN_LIGHT);
		LightController::registerLight(FSIID::MBI_PNEUMATICS_AUTO_FAIL_LIGHT);
		LightController::registerLight(FSIID::MBI_PNEUMATICS_DUAL_BLEED_LIGHT);
		LightController::registerLight(FSIID::MBI_PNEUMATICS_LEFT_BLEED_TRIP_OFF_LIGHT);
		LightController::registerLight(FSIID::MBI_PNEUMATICS_LEFT_PACK_LIGHT);
		LightController::registerLight(FSIID::MBI_PNEUMATICS_LEFT_RAM_DOOR_FULL_OPEN_LIGHT, FSIID::MBI_PNEUMATICS_LEFT_RAM_DOOR_FULL_OPEN_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_PNEUMATICS_LEFT_WING_BODY_OVERHEAT_LIGHT);
		LightController::registerLight(FSIID::MBI_PNEUMATICS_MANUAL_LIGHT);
		LightController::registerLight(FSIID::MBI_PNEUMATICS_OFF_SCHED_DESCENT_LIGHT);
		LightController::registerLight(FSIID::MBI_PNEUMATICS_RIGHT_BLEED_TRIP_OFF_LIGHT);
		LightController::registerLight(FSIID::MBI_PNEUMATICS_RIGHT_PACK_LIGHT);
		LightController::registerLight(FSIID::MBI_PNEUMATICS_RIGHT_RAM_DOOR_FULL_OPEN_LIGHT, FSIID::MBI_PNEUMATICS_RIGHT_RAM_DOOR_FULL_OPEN_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_PNEUMATICS_RIGHT_WING_BODY_OVERHEAT_LIGHT);

		FSIcm::inst->set<bool>(FSIID::MBI_PNEUMATICS_LAMPTEST, false);

		FSIcm::inst->ProcessWrites();
	}

	void fsiOnVarReceive(FSIID id)
	{

	}



}