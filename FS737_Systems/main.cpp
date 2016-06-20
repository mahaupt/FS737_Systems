#include <iostream>

#include "FSToolbox/FSIcm.h"
#include "FSToolbox/TimerManager.h"
#include "FSToolbox/LightController.h"

#include "ELT.h"
#include "AIRCOND.h"
#include "ANTIICE.h"
#include "DOORS.h"
#include "ENGINEECC.h"
#include "FRECSTALLTEST.h"
#include "FUEL.h"
#include "HEAT.h"
#include "LED.h"
#include "OXY.h"
#include "PNEUMATICS.h"
#include "TPANEL.h"
#include "IRS.h"
#include "HYDRAULICS.h"
#include "ELEC.h"
#include "VOICEREC.h"
#include "FIRE.h"
#include "MIP.h"
#include "SYSTEMS_Gear.h"
#include "SYSTEMS_APU.h"

using namespace fssystems;

int main(int argsc, char ** argsv) {
	fstoolbox::FSIcm fsicm("FS737 Systems");
	//enabling timer manager
	fstoolbox::TimerManager timerManager(0.1);
	fstoolbox::LightController lightController;

	//start panel classes
	ELT elt;
	AIRCOND aircond;
	ANTIICE antiice;
	DOORS doors;
	ENGINEECC engineecc;
	FRECSTALLTEST frecstalltest;
	FUEL fuel;
	HEAT heat;
	LED led;
	OXY oxy;
	PNEUMATICS pneumatics;
	TPANEL tpanel;
	IRS irs;
	HYDRAULICS hydraulics;
	ELEC elec;
    VOICEREC voicerec;
    FIRE fire;
    MIP mip;
    SYSTEMS_Gear systems_gear;
    SYSTEMS_APU systems_apu;

	//enable LightController
	LightController::enableUpdate();

	//wair for user input
	std::cin.get();

	return 0;
}
