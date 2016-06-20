#include "ELEC.h"


namespace fssystems
{

	bool AC_Powersource::SwitchOn()
	{
		if (isAvailable)
		{
			isOnline = true;
			return true;
		}
		else return false;
	}

	void AC_Powersource::SwitchOff()
	{
		isOnline = false;
	}
	void AC_Powersource::Available()
	{
		isAvailable = true;
	}

	void AC_Powersource::Unavailable()
	{
		isAvailable = false;
		isOnline = false;
	}


	void IDG::Disconnect()
	{
		isConnected = false;
		assigned_powersource->Unavailable();
	}


	void AC_BUS::connect(AC_Powersource & new_powersource)
	{
		if (new_powersource.isAvailable)
		{
			powersource = &new_powersource;
			isPowered = powersource->SwitchOn();

			//vergleiche Speicheradresse der Klassen
			if (powersource != selected_source) sourceOff = true;
			else sourceOff = false;
		}
	}
	void AC_BUS::disconnect(AC_Powersource & disconnect)
	{
		powersource = &disconnect;
		sourceOff = false;
		isPowered = false;
	}

	void AC_BUS::select(AC_Powersource & new_source)
	{
		if (new_source.isAvailable)
		{
			if (powersource == selected_source) selected_source->SwitchOff();
			selected_source = &new_source;
			connect(new_source);
		}
	}



	AC_Powersource ELEC::disconnected;
	ELEC * ELEC::instance = nullptr;

	ELEC::ELEC() :
		ac_bus1(disconnected), ac_bus2(disconnected), idg1(eng1_gen), idg2(eng2_gen)
	{
		//debug variable
		is_debug = true;

		instance = this;

		//starting FSI Client for IRS
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);
		FSIID wanted_vars[] =
		{
			FSIID::IOS_GRD_PWR_CONNECTED,
			FSIID::SLI_APU_GEN_RTL,
			FSIID::SLI_GEN_1_RTL,
			FSIID::SLI_GEN_2_RTL,

			FSIID::MBI_ELEC_BUS_APU_GEN_1_SWITCH_OFF_POS,
			FSIID::MBI_ELEC_BUS_APU_GEN_1_SWITCH_ON_POS,
			FSIID::MBI_ELEC_BUS_APU_GEN_2_SWITCH_OFF_POS,
			FSIID::MBI_ELEC_BUS_APU_GEN_2_SWITCH_ON_POS,
			FSIID::MBI_ELEC_BUS_BUS_TRANSFER_SWITCH,
			FSIID::MBI_ELEC_BUS_GEN_1_SWITCH_OFF_POS,
			FSIID::MBI_ELEC_BUS_GEN_1_SWITCH_ON_POS,
			FSIID::MBI_ELEC_BUS_GEN_2_SWITCH_OFF_POS,
			FSIID::MBI_ELEC_BUS_GEN_2_SWITCH_ON_POS,
			FSIID::MBI_ELEC_BUS_GRD_PWR_SWITCH,
			FSIID::MBI_ELEC_BUS_L_WIPER_SWITCH_HIGH_POS,
			FSIID::MBI_ELEC_BUS_L_WIPER_SWITCH_INT_POS,
			FSIID::MBI_ELEC_BUS_L_WIPER_SWITCH_LOW_POS,
			FSIID::MBI_ELEC_BUS_L_WIPER_SWITCH_PARK_POS,

			FSIID::MBI_ELEC_IND_AC_SWITCH_APU_GEN_POS,
			FSIID::MBI_ELEC_IND_AC_SWITCH_GEN_1_POS,
			FSIID::MBI_ELEC_IND_AC_SWITCH_GEN_2_POS,
			FSIID::MBI_ELEC_IND_AC_SWITCH_GRD_PWR_POS,
			FSIID::MBI_ELEC_IND_AC_SWITCH_INV_POS,
			FSIID::MBI_ELEC_IND_AC_SWITCH_STBY_PWR_POS,
			FSIID::MBI_ELEC_IND_AC_SWITCH_TEST_POS,
			FSIID::MBI_ELEC_IND_BATTERY_SWITCH,
			FSIID::MBI_ELEC_IND_DC_SWITCH_AUX_BAT_POS,
			FSIID::MBI_ELEC_IND_DC_SWITCH_BAT_BUS_POS,
			FSIID::MBI_ELEC_IND_DC_SWITCH_BAT_POS,
			FSIID::MBI_ELEC_IND_DC_SWITCH_STBY_PWR_POS,
			FSIID::MBI_ELEC_IND_DC_SWITCH_TEST_POS,
			FSIID::MBI_ELEC_IND_DC_SWITCH_TR1_POS,
			FSIID::MBI_ELEC_IND_DC_SWITCH_TR2_POS,
			FSIID::MBI_ELEC_IND_DC_SWITCH_TR3_POS,
			FSIID::MBI_ELEC_IND_GALLEY_POWER_SWITCH,
			FSIID::MBI_ELEC_IND_IFE_SWITCH,
			FSIID::MBI_ELEC_IND_MAINT_SWITCH,

			FSIID::MBI_ELEC_STBY_GEN_1_DISCONNECT_SWITCH,
			FSIID::MBI_ELEC_STBY_GEN_2_DISCONNECT_SWITCH,
			FSIID::MBI_ELEC_STBY_STANDBY_POWER_SWITCH_AUTO_POS,
			FSIID::MBI_ELEC_STBY_STANDBY_POWER_SWITCH_BAT_POS
		};
		FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));

		//GND PWR. APU, ENG GEN testweise alle auf ON
		//FSIcm::inst->set<bool>(FSIID::SLI_APU_GEN_RTL, true);
		FSIcm::inst->set<bool>(FSIID::IOS_GRD_PWR_CONNECTED, true);
		FSIcm::inst->set<bool>(FSIID::SLI_GEN_1_RTL, true);
		FSIcm::inst->set<bool>(FSIID::SLI_GEN_2_RTL, true);

		//standard values
		LightController::registerLight(FSIID::MBI_ELEC_BUS_GRD_PWR_AVAILABLE_LIGHT);
		LightController::registerLight(FSIID::MBI_ELEC_STBY_STANDBY_PWR_OFF_LIGHT, FSIID::MBI_ELEC_STBY_STANDBY_PWR_OFF_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_ELEC_STBY_GEN_1_DRIVE_LIGHT, FSIID::MBI_ELEC_STBY_GEN_1_DRIVE_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_ELEC_STBY_GEN_2_DRIVE_LIGHT, FSIID::MBI_ELEC_STBY_GEN_2_DRIVE_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_ELEC_IND_BAT_DISCHARGE_LIGHT, FSIID::MBI_ELEC_IND_BAT_DISCHARGE_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_ELEC_BUS_APU_GEN_OFF_BUS_LIGHT);
		LightController::registerLight(FSIID::MBI_ELEC_BUS_GEN_1_GEN_OFF_BUS_LIGHT);
		LightController::registerLight(FSIID::MBI_ELEC_BUS_GEN_2_GEN_OFF_BUS_LIGHT);
		LightController::registerLight(FSIID::MBI_ELEC_IND_ELEC_LIGHT, FSIID::MBI_ELEC_IND_ELEC_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_ELEC_IND_TR_UNIT_LIGHT, FSIID::MBI_ELEC_IND_TR_UNIT_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_ELEC_BUS_GEN_1_SOURCE_OFF_LIGHT);
		LightController::registerLight(FSIID::MBI_ELEC_BUS_GEN_1_TRANSFER_BUS_OFF_LIGHT);
		LightController::registerLight(FSIID::MBI_ELEC_BUS_GEN_2_TRANSFER_BUS_OFF_LIGHT);
		LightController::registerLight(FSIID::MBI_ELEC_BUS_GEN_2_SOURCE_OFF_LIGHT);

		FSIcm::inst->set<bool>(FSIID::MBI_ELEC_BUS_LAMPTEST, false);
		FSIcm::inst->set<bool>(FSIID::MBI_ELEC_IND_LAMPTEST, false);
		FSIcm::inst->set<bool>(FSIID::MBI_ELEC_STBY_LAMPTEST, false);

		FSIcm::inst->ProcessWrites();

		simElectrics();
	}


	void ELEC::fsiOnVarReceive(FSIID id)
	{
		instance->onVarReceive(id);
	}


	void ELEC::onVarReceive(FSIID & id)
	{

		//GROUND POWER
		if (id == FSIID::IOS_GRD_PWR_CONNECTED)
		{
			if (FSIcm::inst->get<bool>(FSIID::IOS_GRD_PWR_CONNECTED))
			{
				debug("ELEC GND PWR Connected");
				ext_pwr_l.Available();
				ext_pwr_r.Available();
			}
			else
			{
				debug("ELEC GND PWR Disconnected");
				ext_pwr_l.Unavailable();
				ext_pwr_r.Available();
			}

			//GND PWR available
			LightController::set(FSIID::MBI_ELEC_BUS_GRD_PWR_AVAILABLE_LIGHT, FSIcm::inst->get<bool>(FSIID::IOS_GRD_PWR_CONNECTED));
			simElectrics();
		}

		//GROUND POWER SWITCH
		if (id == FSIID::MBI_ELEC_BUS_GRD_PWR_SWITCH)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_ELEC_BUS_GRD_PWR_SWITCH))
			{
				debug("ELEC GND PWR SWITCH On");
				if (ext_pwr_l.SwitchOn()) ac_bus1.select(ext_pwr_l);
				if (ext_pwr_r.SwitchOn()) ac_bus2.select(ext_pwr_r);
			}
			else
			{
				debug("ELEC GND PWR SWITCH Off");
				ext_pwr_l.SwitchOff();
				ext_pwr_r.SwitchOff();
			}
			simElectrics();
		}

		//battery
		if (id == FSIID::MBI_ELEC_IND_BATTERY_SWITCH)
		{
			if (!FSIcm::inst->get<bool>(FSIID::MBI_ELEC_IND_BATTERY_SWITCH))
			{
				debug("ELEC DC Bat On");
				battery_online = true;
				simElectrics();
			}
			else
			{
				debug("ELEC DC Bat Off");
				battery_online = false;
				simElectrics();
			}


		}

		//STBY Power
		if (id == FSIID::MBI_ELEC_STBY_STANDBY_POWER_SWITCH_AUTO_POS || id == FSIID::MBI_ELEC_STBY_STANDBY_POWER_SWITCH_BAT_POS)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_ELEC_STBY_STANDBY_POWER_SWITCH_AUTO_POS))
			{
				debug("ELEC STBY PWR AUTO");
				sby_pwr_auto = true;
				sby_pwr_bat = false;
			}
			else if (FSIcm::inst->get<bool>(FSIID::MBI_ELEC_STBY_STANDBY_POWER_SWITCH_BAT_POS))
			{
				debug("ELEC STBY PWR BAT");
				sby_pwr_auto = false;
				sby_pwr_bat = true;
			}
			else
			{
				debug("ELEC STBY PWR OFF");
				sby_pwr_bat = false;
				sby_pwr_auto = false;
			}
			simElectrics();
		}

		//IDG_1
		if (id == FSIID::MBI_ELEC_STBY_GEN_1_DISCONNECT_SWITCH)
		{
			if (!FSIcm::inst->get<bool>(FSIID::MBI_ELEC_STBY_GEN_1_DISCONNECT_SWITCH))
			{
				debug("ELEC STBY Gen 1 Connected");
			}
			else
			{
				debug("ELEC STBY Gen 1 Disconnected");
				idg1.Disconnect();
			}
			simElectrics();
		}

		//IDG_2
		if (id == FSIID::MBI_ELEC_STBY_GEN_2_DISCONNECT_SWITCH)
		{
			if (!FSIcm::inst->get<bool>(FSIID::MBI_ELEC_STBY_GEN_2_DISCONNECT_SWITCH))
			{
				debug("ELEC STBY Gen 2 Connected");
			}
			else
			{
				debug("ELEC STBY Gen 2 Disconnected");
				idg2.Disconnect();
			}
			simElectrics();
		}

		//APU GEN 1
		if (id == FSIID::MBI_ELEC_BUS_APU_GEN_1_SWITCH_OFF_POS || id == FSIID::MBI_ELEC_BUS_APU_GEN_1_SWITCH_ON_POS)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_ELEC_BUS_APU_GEN_1_SWITCH_OFF_POS))
			{
				debug("ELEC APU GEN 1 Off");
				apu_gen1.SwitchOff();
			}
			if (FSIcm::inst->get<bool>(FSIID::MBI_ELEC_BUS_APU_GEN_1_SWITCH_ON_POS))
			{
				debug("ELEC APU GEN 1 On");
				if (apu_gen1.SwitchOn())
				{
					ac_bus1.select(apu_gen1);
					ext_pwr_l.SwitchOff();
					ext_pwr_r.SwitchOff();
				}
			}
			simElectrics();
		}

		//APU GEN 2
		if (id == FSIID::MBI_ELEC_BUS_APU_GEN_2_SWITCH_OFF_POS || id == FSIID::MBI_ELEC_BUS_APU_GEN_2_SWITCH_ON_POS)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_ELEC_BUS_APU_GEN_2_SWITCH_OFF_POS))
			{
				debug("ELEC APU GEN 2 Off");
				apu_gen2.SwitchOff();
			}
			if (FSIcm::inst->get<bool>(FSIID::MBI_ELEC_BUS_APU_GEN_2_SWITCH_ON_POS))
			{
				debug("ELEC APU GEN 2 On");
				if (apu_gen2.SwitchOn())
				{
					ac_bus2.select(apu_gen2);
					ext_pwr_l.SwitchOff();
					ext_pwr_r.SwitchOff();
				}
			}

			simElectrics();
		}

		//ENG 1 GEN
		if (id == FSIID::MBI_ELEC_BUS_GEN_1_SWITCH_OFF_POS || id == FSIID::MBI_ELEC_BUS_GEN_1_SWITCH_ON_POS)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_ELEC_BUS_GEN_1_SWITCH_OFF_POS))
			{
				debug("ELEC ENG GEN 1 Off");
				eng1_gen.SwitchOff();
			}
			if (FSIcm::inst->get<bool>(FSIID::MBI_ELEC_BUS_GEN_1_SWITCH_ON_POS))
			{
				debug("ELEC ENG GEN 1 On");
				if (eng1_gen.SwitchOn()) ac_bus1.select(eng1_gen);
			}
			simElectrics();
		}

		//ENG 2 GEN
		if (id == FSIID::MBI_ELEC_BUS_GEN_2_SWITCH_OFF_POS || id == FSIID::MBI_ELEC_BUS_GEN_2_SWITCH_ON_POS)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_ELEC_BUS_GEN_2_SWITCH_OFF_POS))
			{
				debug("ELEC ENG GEN 2 Off");
				eng2_gen.SwitchOff();
			}
			if (FSIcm::inst->get<bool>(FSIID::MBI_ELEC_BUS_GEN_2_SWITCH_ON_POS))
			{
				debug("ELEC ENG GEN 2 On");
				if (eng2_gen.SwitchOn()) ac_bus2.select(eng2_gen);
			}
			simElectrics();
		}

		//BUS TRANSFER
		if (id == FSIID::MBI_ELEC_BUS_BUS_TRANSFER_SWITCH)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_ELEC_BUS_BUS_TRANSFER_SWITCH))
			{
				debug("ELEC BUS TRANSFER AUTO");
				bustransfer_auto = true;
			}
			else
			{
				debug("ELEC BUS TRANSFER OFF");
				bustransfer_auto = false;
			}
			simElectrics();
		}


		//some changes in generator availability
		if (id == FSIID::SLI_APU_GEN_RTL)
		{
			if (FSIcm::inst->get<bool>(FSIID::SLI_APU_GEN_RTL))
			{
				debug("ELEC APU ready to load");
				apu_gen1.Available();
				apu_gen2.Available();
			}
			else
			{
				debug("ELEC APU not ready to load");
				apu_gen1.Unavailable();
				apu_gen2.Unavailable();
			}
			simElectrics();
		}

		if (id == FSIID::SLI_GEN_1_RTL)
		{
			if (FSIcm::inst->get<bool>(FSIID::SLI_GEN_1_RTL) && idg1.isConnected) {
				eng1_gen.Available();
				debug("ELEC GEN 1 ready to load");
			}
			else {
				eng1_gen.Unavailable();
				debug("ELEC GEN 1 not ready to load");
			}
			simElectrics();
		}

		if (id == FSIID::SLI_GEN_2_RTL)
		{
			if (FSIcm::inst->get<bool>(FSIID::SLI_GEN_2_RTL) && idg2.isConnected) {
				eng2_gen.Available();
				debug("ELEC GEN 2 ready to load");
			}
			else {
				eng2_gen.Unavailable();
				debug("ELEC GEN 2 not ready to load");
			}
			simElectrics();
		}
	}



	//
	void ELEC::simElectrics()
	{
		//disconnect powersources if necessary
		if (!ac_bus1.powersource->isOnline)
			ac_bus1.disconnect(disconnected);

		if (!ac_bus2.powersource->isOnline)
			ac_bus2.disconnect(disconnected);

		//auto-transfer
		if (bustransfer_auto)
		{
			if (!ac_bus1.isPowered)
			{
				ac_bus1.connect(*ac_bus2.powersource);
			}
			if (!ac_bus2.isPowered)
			{
				ac_bus2.connect(*ac_bus1.powersource);
			}
		}
		else
		{
			if (ac_bus1.powersource != ac_bus1.selected_source) ac_bus1.disconnect(disconnected);
			if (ac_bus2.powersource != ac_bus2.selected_source) ac_bus2.disconnect(disconnected);
		}

		// Set LEDs for Busses
		LightController::set(FSIID::MBI_ELEC_BUS_GEN_1_TRANSFER_BUS_OFF_LIGHT, !ac_bus1.isPowered);
		LightController::set(FSIID::MBI_ELEC_BUS_GEN_1_SOURCE_OFF_LIGHT, ac_bus1.sourceOff);
		LightController::set(FSIID::MBI_ELEC_BUS_GEN_2_TRANSFER_BUS_OFF_LIGHT, !ac_bus2.isPowered);
		LightController::set(FSIID::MBI_ELEC_BUS_GEN_2_SOURCE_OFF_LIGHT, ac_bus2.sourceOff);

		// Set LEDs of IDGs
		LightController::set(FSIID::MBI_ELEC_STBY_GEN_1_DRIVE_LIGHT, !eng1_gen.isAvailable);
		LightController::set(FSIID::MBI_ELEC_STBY_GEN_2_DRIVE_LIGHT, !eng2_gen.isAvailable);

		//BAT DISCHARGE LIGHT
		if (battery_online && !ac_bus1.isPowered && !ac_bus2.isPowered)
		{
			LightController::set(FSIID::MBI_ELEC_IND_BAT_DISCHARGE_LIGHT, true);
		}
		else
		{
			LightController::set(FSIID::MBI_ELEC_IND_BAT_DISCHARGE_LIGHT, false);
		}

		//APU OFF BUS LIGHT
		if (apu_gen1.isAvailable && apu_gen2.isAvailable && !apu_gen1.isOnline && !apu_gen2.isOnline)
		{
			LightController::set(FSIID::MBI_ELEC_BUS_APU_GEN_OFF_BUS_LIGHT, true);
		}
		else
		{
			LightController::set(FSIID::MBI_ELEC_BUS_APU_GEN_OFF_BUS_LIGHT, false);
		}

		//ENG 1 GEN BUS OFF LIGHT
		if (eng1_gen.isOnline)
		{
			LightController::set(FSIID::MBI_ELEC_BUS_GEN_1_GEN_OFF_BUS_LIGHT, false);
		}
		else
		{
			LightController::set(FSIID::MBI_ELEC_BUS_GEN_1_GEN_OFF_BUS_LIGHT, true);
		}

		//ENG 2 GEN BUS OFF LIGHT
		if (eng2_gen.isOnline)
		{
			LightController::set(FSIID::MBI_ELEC_BUS_GEN_2_GEN_OFF_BUS_LIGHT, false);
		}
		else
		{
			LightController::set(FSIID::MBI_ELEC_BUS_GEN_2_GEN_OFF_BUS_LIGHT, true);
		}

		//Check AC Bus 1 Power (heavy load systems e.g. displays)
		if (ac_bus1.isPowered)
		{
			//set SLI Voltage
			FSIcm::inst->set<float>(FSIID::SLI_AC_XFR_BUS_1_PHASE_1_VOLTAGE, 110);

			//displays on
			switchAC1Systems(true);
		}
		else
		{
			FSIcm::inst->set<float>(FSIID::SLI_AC_XFR_BUS_1_PHASE_1_VOLTAGE, 0);

			//displays off
			switchAC1Systems(false);
		}

		// Check AC Bus 2 Power (essentially same systems as AC Bus 1) 
		// ### TO-DO: Switch single systems depending on which AC Bus is powered
		if (ac_bus2.isPowered)
		{
			FSIcm::inst->set<float>(FSIID::SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE, 110);

			//displays on
			switchAC2Systems(true);
		}
		else
		{
			FSIcm::inst->set<float>(FSIID::SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE, 0);

			//displays off
			switchAC2Systems(false);
		}

		// DC BUSSES

		// DC BUS 1
		if (ac_bus1.isPowered)
		{
			//fsi.SLI_DC_BUS_1_VOLTAGE = 28
		}
		else; //fsi.SLI_DC_BUS_1_VOLTAGE = 0;

			  // DC BUS 2
		if (ac_bus2.isPowered)
		{
			//fsi.SLI_DC_BUS_2_VOLTAGE = 28
		}
		else; //fsi.SLI_DC_BUS_2_VOLTAGE = 0;

			  // DC SWITCHED HOT BATTERY BUS
		if (battery_online)
		{
			// fsi.SLI_DC_SWITCHED_HOT_BATTERY_BUS_VOLTAGE = 24;
		}
		else; // fsi.SLI_DC_SWITCHED_HOT_BATTERY_BUS_VOLTAGE = 0;

			  // DC BATTERY BUS (it is supposed that battery capacity is infinite)
		if (battery_online || ac_bus1.isPowered || ac_bus2.isPowered)
		{
			//debug("BATTERY BUS - some power available");
			if (ac_bus1.isPowered || ac_bus2.isPowered)
			{
				FSIcm::inst->set<float>(FSIID::SLI_BAT_BUS_VOLTAGE, 28);
				LightController::set(FSIID::MBI_ELEC_IND_BAT_DISCHARGE_LIGHT, false);
			}
			else if (battery_online)
			{
				FSIcm::inst->set<float>(FSIID::SLI_BAT_BUS_VOLTAGE, 24);
				LightController::set(FSIID::MBI_ELEC_IND_BAT_DISCHARGE_LIGHT, true);
			}
			switchDCSystems(true);
		}
		else
		{
			//debug("BATTERY BUS - no power available");
			FSIcm::inst->set<float>(FSIID::SLI_BAT_BUS_VOLTAGE, 0);
			switchDCSystems(false);
		}

		// STBY POWER  (it is supposed that battery capacity is infinite)
		if ((sby_pwr_auto && (battery_online || ac_bus1.isPowered)) || (sby_pwr_bat && battery_online))
		{
			FSIcm::inst->set<float>(FSIID::SLI_AC_STBY_BUS_PHASE_1_VOLTAGE, 110); // AC STANDBY BUS
			if (ac_bus1.isPowered)  // STBY BUS RUNNING ON AC BUS 1
			{
				FSIcm::inst->set<float>(FSIID::SLI_DC_STBY_BUS_SECT_1_VOLTAGE, 28); // DC STANDBY BUS                    
			}
			else // STBY BUS RUNNING ON BATTERY
			{
				FSIcm::inst->set<float>(FSIID::SLI_DC_STBY_BUS_SECT_1_VOLTAGE, 24); // DC STANDBY BUS
			}
		}
		else
		{
			FSIcm::inst->set<float>(FSIID::SLI_AC_STBY_BUS_PHASE_1_VOLTAGE, 0); // AC STANDBY BUS
			FSIcm::inst->set<float>(FSIID::SLI_DC_STBY_BUS_SECT_1_VOLTAGE, 0); // DC STANDBY BUS
		}

		// STBY PWR OFF LIGHT
		if (FSIcm::inst->get<float>(FSIID::SLI_BAT_BUS_VOLTAGE) == 0 || FSIcm::inst->get<float>(FSIID::SLI_DC_STBY_BUS_SECT_1_VOLTAGE) == 0 || FSIcm::inst->get<float>(FSIID::SLI_AC_STBY_BUS_PHASE_1_VOLTAGE) == 0)
		{
			LightController::set(FSIID::MBI_ELEC_STBY_STANDBY_PWR_OFF_LIGHT, true);
		}
		else LightController::set(FSIID::MBI_ELEC_STBY_STANDBY_PWR_OFF_LIGHT, false);

		FSIcm::inst->ProcessWrites();
	}


	//switch AC Systems on BUS 1 on / Off
	void ELEC::switchAC1Systems(bool power)
	{
		//all displays
		FSIcm::inst->set<bool>(FSIID::INT_POWER_EICAS, power);
		FSIcm::inst->set<bool>(FSIID::INT_POWER_ISFD, power);
		FSIcm::inst->set<bool>(FSIID::INT_POWER_LDU, power);
		FSIcm::inst->set<bool>(FSIID::INT_POWER_ND_CPT, power);
		FSIcm::inst->set<bool>(FSIID::INT_POWER_ND_FO, power);
		FSIcm::inst->set<bool>(FSIID::INT_POWER_PFD_CPT, power);
		FSIcm::inst->set<bool>(FSIID::INT_POWER_PFD_FO, power);
		FSIcm::inst->set<bool>(FSIID::INT_POWER_SRMI, power);
	}

	//switch AC Systems on BUS 2 on / Off
	void ELEC::switchAC2Systems(bool power)
	{
	}

	void ELEC::switchDCSystems(bool power)
	{
		FSIcm::inst->set<bool>(FSIID::CPF_MCP_POWER, power);

		//all status lights
		if (power)
		{
			LightController::setLightPower(true);
			//debug("DC Systems Power ON");
		}
		else
		{
			LightController::setLightPower(false);
			//debug("DC Systems Power OFF");
		}
	}

}