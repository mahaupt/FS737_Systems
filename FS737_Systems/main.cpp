#include <iostream>

#include "FSInterface/FSIClient.h"

fsinterface::FSIClient *fsi = NULL;

//declare function (defined below)
void onReceiveVar(fsinterface::FSIID id);

int main(char * args, int argv) {
	//create FSI Client in heap
	fsi = new fsinterface::FSIClient("FSI Test");

	//register the Callback so that the function below is called
	//every time a subscribed FSI variable changes
	fsi->RegisterCallback(onReceiveVar);

	//declare which FSI variables to receive
	fsinterface::FSIID wanted_fsi_vars[] =
	{
		fsinterface::FSIID::MBI_ELEC_BUS_GRD_PWR_SWITCH,
		fsinterface::FSIID::MBI_ELEC_BUS_APU_GEN_1_SWITCH_ON_POS
	};
	fsi->DeclareAsWanted(wanted_fsi_vars, sizeof(wanted_fsi_vars) / sizeof(fsinterface::FSIID));

	//send the changes to the Server
	fsi->ProcessWrites();

	//wair for user input
	std::cin.get();

	//clear fsi client from heap
	delete fsi;

	return 0;
}


void onReceiveVar(fsinterface::FSIID id) {
	bool switch_pos = false;
	switch (id)
	{
	case fsinterface::FSIID::MBI_ELEC_BUS_GRD_PWR_SWITCH:

		//get switch position
		switch_pos = fsi->get<bool>(fsinterface::FSIID::MBI_ELEC_BUS_GRD_PWR_SWITCH);
		std::cout << "Received: MBI_ELEC_BUS_GRD_PWR_SWITCH: " << switch_pos << std::endl;

		//set light position
		fsi->set<bool>(fsinterface::FSIID::MBI_ELEC_BUS_GRD_PWR_AVAILABLE_LIGHT, switch_pos);
		fsi->ProcessWrites(); //send update to server

		break;
	}
}