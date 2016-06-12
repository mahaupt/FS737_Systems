#include "DOORS.h"

namespace fssystems
{

	DOORS::DOORS()
	{
		//debug variable
		is_debug = true;

		//starting FSI Client for IRS
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);

		//standard values
		LightController::registerLight(FSIID::MBI_DOOR_AFT_CARGO_LIGHT, FSIID::MBI_DOOR_AFT_CARGO_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_DOOR_AFT_ENTRY_LIGHT, FSIID::MBI_DOOR_AFT_ENTRY_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_DOOR_AFT_SERVICE_LIGHT, FSIID::MBI_DOOR_AFT_SERVICE_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_DOOR_AIRSTAIR_LIGHT, FSIID::MBI_DOOR_AIRSTAIR_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_DOOR_EQUIP_LIGHT, FSIID::MBI_DOOR_EQUIP_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_DOOR_FWD_CARGO_LIGHT, FSIID::MBI_DOOR_FWD_CARGO_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_DOOR_FWD_ENTRY_LIGHT, FSIID::MBI_DOOR_FWD_ENTRY_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_DOOR_FWD_SERVICE_LIGHT, FSIID::MBI_DOOR_FWD_SERVICE_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_DOOR_LEFT_AFT_OVERWING_LIGHT, FSIID::MBI_DOOR_LEFT_AFT_OVERWING_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_DOOR_LEFT_FWD_OVERWING_LIGHT, FSIID::MBI_DOOR_LEFT_FWD_OVERWING_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_DOOR_RIGHT_AFT_OVERWING_LIGHT, FSIID::MBI_DOOR_RIGHT_AFT_OVERWING_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_DOOR_RIGHT_FWD_OVERWING_LIGHT, FSIID::MBI_DOOR_RIGHT_FWD_OVERWING_LIGHT_DIMMED);

		FSIcm::inst->set<bool>(FSIID::MBI_DOOR_LAMPTEST, false);

		FSIcm::inst->ProcessWrites();
	}


	void DOORS::fsiOnVarReceive(FSIID id)
	{

	}

}