#include <iostream>

#include "FSToolbox/FSIcm.h"
#include "FSToolbox/TimerManager.h"
#include "FSToolbox/LightController.h"

#include "ELT.h"

using namespace fssystems;

int main(char * args, int argv) {
	fstoolbox::FSIcm fsicm("Overheadpanel Systems");
	//enabling timer manager
	fstoolbox::TimerManager timerManager(0.1);
	fstoolbox::LightController lightController;

	//start panel classes
	ELT elt;


	//enable LightController
	LightController::enableUpdate();

	//wair for user input
	std::cin.get();

	return 0;
}
