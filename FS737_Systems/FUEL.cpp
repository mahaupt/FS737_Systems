#include "FUEL.h"

namespace fssystems
{
    FuelPump::FuelPump() :
        pressurize_timer(1.5, pressurizeCallback, (void*)this),
        depressurize_timer(0.5, depressurizeCallback, (void *)this)
    {
        pump_status = FUEL_PMP_STAT::PUMP_OFFLINE;
        fuel_level = 1;

        TimerManager::addTimer(pressurize_timer);
        TimerManager::addTimer(depressurize_timer);
    }

    void FuelPump::setPower(bool value) {
        if (value != (pump_status & FUEL_PMP_STAT::PUMP_ONLINE)) {
            if (value) 
            {
                //set pump online
                pump_status |= FUEL_PMP_STAT::PUMP_ONLINE;
                pump_status &=  ~FUEL_PMP_STAT::PUMP_NO_FUEL_CUTOUT;
                pressurize_timer.Start();
                depressurize_timer.Reset();
            } else {
                //set pump offline
                pump_status &= ~FUEL_PMP_STAT::PUMP_ONLINE;
                if (fuel_level <= 0) {
                    pump_status |=  FUEL_PMP_STAT::PUMP_NO_FUEL_CUTOUT;
                }
                
                pressurize_timer.Reset();
                depressurize_timer.Start();
            }
        }

    }

    void FuelPump::setFuelLevel(float value)
    { 
        fuel_level = value;
        
        //sets cutout if fuel level zero
        //ACHTUNG: Mit Standaloneserver ist der Treibstoff immer null! 
        if (fuel_level <= 0) {
        //no fuel
            pump_status |= FUEL_PMP_STAT::PUMP_NO_FUEL_CUTOUT;
            pump_status &= ~FUEL_PMP_STAT::PUMP_PRESSURE;
        } else {
            pump_status &= ~FUEL_PMP_STAT::PUMP_NO_FUEL_CUTOUT;
        }
    }

    void FuelPump::pressurizeCallback(void * instance)
    {
        if (((FuelPump*)instance)->fuel_level <= 0) {
            //no fuel
            ((FuelPump*)instance)->pump_status &= ~FUEL_PMP_STAT::PUMP_PRESSURE;
            ((FuelPump*)instance)->pump_status |= FUEL_PMP_STAT::PUMP_NO_FUEL_CUTOUT;
        } else {
            ((FuelPump*)instance)->pump_status |= FUEL_PMP_STAT::PUMP_PRESSURE;
            ((FuelPump*)instance)->pump_status &= ~FUEL_PMP_STAT::PUMP_NO_FUEL_CUTOUT;
        }
        FUEL::sim_fuel_st();
    }

    void FuelPump::depressurizeCallback(void * instance)
    {
        ((FuelPump*)instance)->pump_status &= ~FUEL_PMP_STAT::PUMP_PRESSURE;
        FUEL::sim_fuel_st();
    }


	FUEL * FUEL::instance = nullptr;

	FUEL::FUEL()
	{
		//debug variable
		is_debug = true;

		instance = this;

		//starting FSI Client for IRS
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);
		FSIID wanted_vars[] =
		{
			FSIID::MBI_FUEL_CROSSFEED_SWITCH,
			FSIID::MBI_FUEL_CTR_LEFT_PUMP_SWITCH,
			FSIID::MBI_FUEL_CTR_RIGHT_PUMP_SWITCH,
			FSIID::MBI_FUEL_LEFT_AFT_PUMP_SWITCH,
			FSIID::MBI_FUEL_LEFT_FWD_PUMP_SWITCH,
			FSIID::MBI_FUEL_RIGHT_AFT_PUMP_SWITCH,
			FSIID::MBI_FUEL_RIGHT_FWD_PUMP_SWITCH,

            FSIID::SLI_AC_XFR_BUS_1_PHASE_1_VOLTAGE,
            FSIID::SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE,

            FSIID::FSI_FUEL_CENTRE_LEVEL_PERCENT,
            FSIID::FSI_FUEL_LEFT_MAIN_LEVEL_PERCENT,
            FSIID::FSI_FUEL_RIGHT_MAIN_LEVEL_PERCENT,
            FSIID::FSI_ALTITUDE
		};
		FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));

		//standard values
		LightController::registerLight(FSIID::MBI_FUEL_CROSSFEED_VALVE_OPEN_LIGHT, FSIID::MBI_FUEL_CROSSFEED_VALVE_OPEN_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_FUEL_CTR_LEFT_PUMP_LOW_PRESSURE_LIGHT);
		LightController::registerLight(FSIID::MBI_FUEL_CTR_RIGHT_PUMP_LOW_PRESSURE_LIGHT);
		LightController::registerLight(FSIID::MBI_FUEL_LEFT_AFT_PUMP_LOW_PRESSURE_LIGHT);
		LightController::registerLight(FSIID::MBI_FUEL_LEFT_ENG_VALVE_CLOSED_LIGHT, FSIID::MBI_FUEL_LEFT_ENG_VALVE_CLOSED_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_FUEL_LEFT_FILTER_BYPASS_LIGHT);
		LightController::registerLight(FSIID::MBI_FUEL_LEFT_FWD_PUMP_LOW_PRESSURE_LIGHT);
		LightController::registerLight(FSIID::MBI_FUEL_LEFT_SPAR_VALVE_CLOSED_LIGHT, FSIID::MBI_FUEL_LEFT_SPAR_VALVE_CLOSED_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_FUEL_RIGHT_AFT_PUMP_LOW_PRESSURE_LIGHT);
		LightController::registerLight(FSIID::MBI_FUEL_RIGHT_ENG_VALVE_CLOSED_LIGHT, FSIID::MBI_FUEL_RIGHT_ENG_VALVE_CLOSED_LIGHT_DIMMED);
		LightController::registerLight(FSIID::MBI_FUEL_RIGHT_FILTER_BYPASS_LIGHT);
		LightController::registerLight(FSIID::MBI_FUEL_RIGHT_FWD_PUMP_LOW_PRESSURE_LIGHT);
		LightController::registerLight(FSIID::MBI_FUEL_RIGHT_SPAR_VALVE_CLOSED_LIGHT, FSIID::MBI_FUEL_RIGHT_SPAR_VALVE_CLOSED_LIGHT_DIMMED);

		FSIcm::inst->set<bool>(FSIID::MBI_FUEL_LAMPTEST, false);

		FSIcm::inst->ProcessWrites();
        
        /*pmp_ctr_l.setFuelLevel(0);
        pmp_ctr_r.setFuelLevel(0);
        pmp_l_aft.setFuelLevel(50);
        pmp_l_fwd.setFuelLevel(50);
        pmp_r_aft.setFuelLevel(50);
        pmp_r_fwd.setFuelLevel(50);*/
	}

	void FUEL::fsiOnVarReceive(FSIID id) {
		instance->onVarReceive(id);
	}

	void FUEL::onVarReceive(FSIID & id)
	{
		//CROSSFEED
		if (id == FSIID::MBI_FUEL_CROSSFEED_SWITCH)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_FUEL_CROSSFEED_SWITCH))
			{
				debug("FUEL Crossfeed On");
			}
			else
			{
				debug("FUEL Crossfeed Off");
			}

			//ELT light
			LightController::set(FSIID::MBI_FUEL_CROSSFEED_VALVE_OPEN_LIGHT, FSIcm::inst->get<bool>(FSIID::MBI_FUEL_CROSSFEED_SWITCH));
			LightController::ProcessWrites();
		}

		//FUEL CTR L
		if (id == FSIID::MBI_FUEL_CTR_LEFT_PUMP_SWITCH || id == FSIID::SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_FUEL_CTR_LEFT_PUMP_SWITCH) && FSIcm::inst->get<float>(FSIID::SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE) > 50)
			{
				debug("FUEL CTR LEFT PUMP On");
                pmp_ctr_l.setPower(true);
			}
			else
			{
				debug("FUEL CTR LEFT PUMP Off");
                pmp_ctr_l.setPower(false);
			}
            sim_fuel();
		}

		//FUEL CTR R
		if (id == FSIID::MBI_FUEL_CTR_RIGHT_PUMP_SWITCH || id == FSIID::SLI_AC_XFR_BUS_1_PHASE_1_VOLTAGE)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_FUEL_CTR_RIGHT_PUMP_SWITCH) && FSIcm::inst->get<float>(FSIID::SLI_AC_XFR_BUS_1_PHASE_1_VOLTAGE) > 50)
			{
				debug("FUEL CTR RIGHT PUMP On");
                pmp_ctr_r.setPower(true);
			}
			else
			{
				debug("FUEL CTR RIGHT PUMP Off");
                pmp_ctr_r.setPower(false);
			}
            sim_fuel();
		}


		//FUEL AFT L
		if (id == FSIID::MBI_FUEL_LEFT_AFT_PUMP_SWITCH || id == FSIID::SLI_AC_XFR_BUS_1_PHASE_1_VOLTAGE)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_FUEL_LEFT_AFT_PUMP_SWITCH) && FSIcm::inst->get<float>(FSIID::SLI_AC_XFR_BUS_1_PHASE_1_VOLTAGE) > 50)
			{
				debug("FUEL AFT LEFT PUMP On");
                pmp_l_aft.setPower(true);
			}
			else
			{
				debug("FUEL AFT LEFT PUMP Off");
                pmp_l_aft.setPower(false);
			}
            sim_fuel();
		}

		//FUEL AFT R
		if (id == FSIID::MBI_FUEL_RIGHT_AFT_PUMP_SWITCH || id == FSIID::SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_FUEL_RIGHT_AFT_PUMP_SWITCH) && FSIcm::inst->get<float>(FSIID::SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE) > 50)
			{
				debug("FUEL AFT RIGHT PUMP On");
                pmp_r_aft.setPower(true);
			}
			else
			{
				debug("FUEL AFT RIGHT PUMP Off");
                pmp_r_aft.setPower(false);
			}
            sim_fuel();
		}

		//FUEL FWD L
		if (id == FSIID::MBI_FUEL_LEFT_FWD_PUMP_SWITCH || id == FSIID::SLI_AC_XFR_BUS_1_PHASE_1_VOLTAGE)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_FUEL_LEFT_FWD_PUMP_SWITCH) && FSIcm::inst->get<float>(FSIID::SLI_AC_XFR_BUS_1_PHASE_1_VOLTAGE) > 50)
			{
				debug("FUEL FWD LEFT PUMP On");
                pmp_l_fwd.setPower(true);
			}
			else
			{
				debug("FUEL FWD LEFT PUMP Off");
                pmp_l_fwd.setPower(false);
			}
            sim_fuel();
		}

		//FUEL FWD R
		if (id == FSIID::MBI_FUEL_RIGHT_FWD_PUMP_SWITCH || id == FSIID::SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_FUEL_RIGHT_FWD_PUMP_SWITCH) && FSIcm::inst->get<float>(FSIID::SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE) > 50)
			{
				debug("FUEL FWD RIGHT PUMP On");
                pmp_r_fwd.setPower(true);
			}
			else
			{
				debug("FUEL FWD RIGHT PUMP Off");
                pmp_r_fwd.setPower(false);
			}
            sim_fuel();
		}

        //FUEL LEVEL CTR
        if (id == FSIID::FSI_FUEL_CENTRE_LEVEL_PERCENT) {
            pmp_ctr_l.setFuelLevel(FSIcm::inst->get<float>(FSIID::FSI_FUEL_CENTRE_LEVEL_PERCENT));
            pmp_ctr_r.setFuelLevel(FSIcm::inst->get<float>(FSIID::FSI_FUEL_CENTRE_LEVEL_PERCENT));
            sim_fuel();
        }

        //FUEL LEVEL LEFT
        if (id == FSIID::FSI_FUEL_LEFT_MAIN_LEVEL_PERCENT) {
            pmp_l_aft.setFuelLevel(FSIcm::inst->get<float>(FSIID::FSI_FUEL_LEFT_MAIN_LEVEL_PERCENT));
            pmp_l_fwd.setFuelLevel(FSIcm::inst->get<float>(FSIID::FSI_FUEL_LEFT_MAIN_LEVEL_PERCENT));
            sim_fuel();
        }

        //FUEL LEVEL RIGHT
        if (id == FSIID::FSI_FUEL_RIGHT_MAIN_LEVEL_PERCENT) {
            pmp_r_aft.setFuelLevel(FSIcm::inst->get<float>(FSIID::FSI_FUEL_RIGHT_MAIN_LEVEL_PERCENT));
            pmp_r_fwd.setFuelLevel(FSIcm::inst->get<float>(FSIID::FSI_FUEL_RIGHT_MAIN_LEVEL_PERCENT));
            sim_fuel();
        }
	}



    void FUEL::sim_fuel_st() {
        instance->sim_fuel();
    }

    void FUEL::sim_fuel() {
        //light off when pressure or no fuel and pump offline
        // CTR L
        if (pmp_ctr_l.pump_status & FUEL_PMP_STAT::PUMP_PRESSURE ||
        (pmp_ctr_l.pump_status & FUEL_PMP_STAT::PUMP_NO_FUEL_CUTOUT && !(pmp_ctr_l.pump_status & FUEL_PMP_STAT::PUMP_ONLINE))) {
            LightController::set(FSIID::MBI_FUEL_CTR_LEFT_PUMP_LOW_PRESSURE_LIGHT, false);
        }
        else {
            LightController::set(FSIID::MBI_FUEL_CTR_LEFT_PUMP_LOW_PRESSURE_LIGHT, true);
        }

        // CTR R
        if (pmp_ctr_r.pump_status & FUEL_PMP_STAT::PUMP_PRESSURE ||
            (pmp_ctr_r.pump_status & FUEL_PMP_STAT::PUMP_NO_FUEL_CUTOUT && !(pmp_ctr_r.pump_status & FUEL_PMP_STAT::PUMP_ONLINE))) {
            LightController::set(FSIID::MBI_FUEL_CTR_RIGHT_PUMP_LOW_PRESSURE_LIGHT, false);
        }
        else {
            LightController::set(FSIID::MBI_FUEL_CTR_RIGHT_PUMP_LOW_PRESSURE_LIGHT, true);
        }

        // AFT L
        if (pmp_l_aft.pump_status & FUEL_PMP_STAT::PUMP_PRESSURE ||
            (pmp_l_aft.pump_status & FUEL_PMP_STAT::PUMP_NO_FUEL_CUTOUT && !(pmp_l_aft.pump_status & FUEL_PMP_STAT::PUMP_ONLINE))) {
            LightController::set(FSIID::MBI_FUEL_LEFT_AFT_PUMP_LOW_PRESSURE_LIGHT, false);
        }
        else {
            LightController::set(FSIID::MBI_FUEL_LEFT_AFT_PUMP_LOW_PRESSURE_LIGHT, true);
        }

        // AFT R
        if (pmp_r_aft.pump_status & FUEL_PMP_STAT::PUMP_PRESSURE ||
            (pmp_r_aft.pump_status & FUEL_PMP_STAT::PUMP_NO_FUEL_CUTOUT && !(pmp_r_aft.pump_status & FUEL_PMP_STAT::PUMP_ONLINE))) {
            LightController::set(FSIID::MBI_FUEL_RIGHT_AFT_PUMP_LOW_PRESSURE_LIGHT, false);
        }
        else {
            LightController::set(FSIID::MBI_FUEL_RIGHT_AFT_PUMP_LOW_PRESSURE_LIGHT, true);
        }

        // FWD L
        if (pmp_l_fwd.pump_status & FUEL_PMP_STAT::PUMP_PRESSURE ||
            (pmp_l_fwd.pump_status & FUEL_PMP_STAT::PUMP_NO_FUEL_CUTOUT && !(pmp_l_fwd.pump_status & FUEL_PMP_STAT::PUMP_ONLINE))) {
            LightController::set(FSIID::MBI_FUEL_LEFT_FWD_PUMP_LOW_PRESSURE_LIGHT, false);
        }
        else {
            LightController::set(FSIID::MBI_FUEL_LEFT_FWD_PUMP_LOW_PRESSURE_LIGHT, true);
        }

        // FWD R
        if (pmp_r_fwd.pump_status & FUEL_PMP_STAT::PUMP_PRESSURE ||
            (pmp_r_fwd.pump_status & FUEL_PMP_STAT::PUMP_NO_FUEL_CUTOUT && !(pmp_r_fwd.pump_status & FUEL_PMP_STAT::PUMP_ONLINE))) {
            LightController::set(FSIID::MBI_FUEL_RIGHT_FWD_PUMP_LOW_PRESSURE_LIGHT, false);
        }
        else {
            LightController::set(FSIID::MBI_FUEL_RIGHT_FWD_PUMP_LOW_PRESSURE_LIGHT, true);
        }

        //Fuel_AVAILABLE
        bool left_fuel = false;
        bool right_fuel = false;

        if ((pmp_ctr_l.pump_status | pmp_l_aft.pump_status | pmp_l_fwd.pump_status) & FUEL_PMP_STAT::PUMP_PRESSURE) {
            left_fuel = true;
        }
        if ((pmp_ctr_r.pump_status | pmp_r_aft.pump_status | pmp_r_fwd.pump_status) & FUEL_PMP_STAT::PUMP_PRESSURE) {
            right_fuel = true;
        }

        //crossfeed
        if (FSIcm::inst->get<bool>(FSIID::MBI_FUEL_CROSSFEED_SWITCH)) {
            if (left_fuel || right_fuel) {
                left_fuel = true;
                right_fuel = true;
            }
        }

        //engine suction
        if (!left_fuel || !right_fuel) {
            //below 20.000 ft engine suction is enough for fuel supply
            if (FSIcm::inst->get<float>(FSIID::FSI_ALTITUDE) < 20000) {
                float ctr_fuel_level = FSIcm::inst->get<float>(FSIID::FSI_FUEL_CENTRE_LEVEL_PERCENT);
                float left_fuel_level = FSIcm::inst->get<float>(FSIID::FSI_FUEL_LEFT_MAIN_LEVEL_PERCENT);
                float right_fuel_level = FSIcm::inst->get<float>(FSIID::FSI_FUEL_RIGHT_MAIN_LEVEL_PERCENT);

                if (ctr_fuel_level > 0 || left_fuel_level > 0) {
                    left_fuel = true;
                }
                if (ctr_fuel_level > 0 || right_fuel_level > 0) {
                    right_fuel = true;
                }
            }
        }

        //write FSI parameter for fuel availability
        if (left_fuel) {
            FSIcm::inst->set<bool>(FSIID::SLI_FUEL_ENG1_AVAIL, true);
            FSIcm::inst->set<bool>(FSIID::SLI_FUEL_APU_AVAIL, true);
        }
        else {
            FSIcm::inst->set<bool>(FSIID::SLI_FUEL_ENG1_AVAIL, false);
            FSIcm::inst->set<bool>(FSIID::SLI_FUEL_APU_AVAIL, false);
        }
        if (right_fuel) {
            FSIcm::inst->set<bool>(FSIID::SLI_FUEL_ENG2_AVAIL, true);
        }
        else {
            FSIcm::inst->set<bool>(FSIID::SLI_FUEL_ENG2_AVAIL, false);
        }



        LightController::ProcessWrites();
    }

}