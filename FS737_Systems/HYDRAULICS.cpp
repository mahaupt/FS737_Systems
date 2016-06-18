#include "HYDRAULICS.h"

namespace fssystems
{
    HYD_Pump::HYD_Pump() :
        timer_pressurize(1.5, callback_pressurize, (void*)this), timer_depressurize(0.5, callback_depressurize, (void*)this)
    {
        pump_status = 0; // PUMP Offline

        //register timer
        TimerManager::addTimer(timer_pressurize);
        TimerManager::addTimer(timer_depressurize);
    }
    void HYD_Pump::callback_pressurize(void * inst) {
        ((HYD_Pump*)inst)->pump_status |= HYD_PUMP_STATUS::HYD_PUMP_PRESSURE;
        HYDRAULICS::sim_hydraulics_st();
    }
    void HYD_Pump::callback_depressurize(void * inst) {
        ((HYD_Pump*)inst)->pump_status &= ~HYD_PUMP_STATUS::HYD_PUMP_PRESSURE;
        HYDRAULICS::sim_hydraulics_st();
    }
    void HYD_Pump::setPower(bool value) {
        //pump status have to be switched
        if (value != (pump_status & HYD_PUMP_STATUS::HYD_PUMP_ONLINE)) 
        {
            if (value) {
                //start pump - pressurize
                pump_status |= HYD_PUMP_STATUS::HYD_PUMP_ONLINE;
                timer_pressurize.Start();
                timer_depressurize.Reset();
            }
            else 
            {
                pump_status &= ~HYD_PUMP_STATUS::HYD_PUMP_ONLINE;
                timer_pressurize.Reset();
                timer_depressurize.Start();
            }
        }
    }


	HYDRAULICS * HYDRAULICS::instance = nullptr;

	HYDRAULICS::HYDRAULICS()
	{
		//debug variable
		is_debug = true;

		instance = this;

		//starting FSI Client for IRS
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);
		FSIID wanted_vars[] =
		{
			FSIID::SLI_GEN_1_RTL,
			FSIID::SLI_GEN_2_RTL,
			FSIID::SLI_AC_XFR_BUS_1_PHASE_1_VOLTAGE,
			FSIID::SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE,

			FSIID::MBI_HYDRAULICS_ELEC_1_SWITCH,
			FSIID::MBI_HYDRAULICS_ELEC_2_SWITCH,
			FSIID::MBI_HYDRAULICS_ENG_1_SWITCH,
			FSIID::MBI_HYDRAULICS_ENG_2_SWITCH,

            FSIID::MALFX_ELEC_HYD_PUMP_A_OVERHEAT_ACTIVE,
            FSIID::MALFX_ELEC_HYD_PUMP_B_OVERHEAT_ACTIVE
		};
		FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));

		//standard values
		LightController::registerLight(FSIID::MBI_HYDRAULICS_ELEC_1_LOW_PRESSURE_LIGHT);
		LightController::registerLight(FSIID::MBI_HYDRAULICS_ELEC_1_OVERHEAT_LIGHT);
		LightController::registerLight(FSIID::MBI_HYDRAULICS_ELEC_2_LOW_PRESSURE_LIGHT);
		LightController::registerLight(FSIID::MBI_HYDRAULICS_ELEC_2_OVERHEAT_LIGHT);
		LightController::registerLight(FSIID::MBI_HYDRAULICS_ENG_1_LOW_PRESSURE_LIGHT);
		LightController::registerLight(FSIID::MBI_HYDRAULICS_ENG_2_LOW_PRESSURE_LIGHT);

		FSIcm::inst->set<bool>(FSIID::MBI_HYDRAULICS_LAMPTEST, false);

		FSIcm::inst->ProcessWrites();
	}

	void HYDRAULICS::fsiOnVarReceive(FSIID id) {
		instance->onVarReceive(id);
	}

	void HYDRAULICS::onVarReceive(FSIID & id)
	{
		//ELEC 1
		if (id == FSIID::MBI_HYDRAULICS_ELEC_1_SWITCH || id == FSIID::SLI_AC_XFR_BUS_1_PHASE_1_VOLTAGE)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_HYDRAULICS_ELEC_1_SWITCH) && FSIcm::inst->get<float>(FSIID::SLI_AC_XFR_BUS_1_PHASE_1_VOLTAGE) > 50) {
				debug("HYDRAULICS ELEC 1 On");
                elec1_hyd.setPower(true);
			}
			else {
				debug("HYDRAULICS ELEC 1 Off");
                elec1_hyd.setPower(false);
			}
			sim_hydraulics();
		}

		//ELEC 2
		if (id == FSIID::MBI_HYDRAULICS_ELEC_2_SWITCH || id == FSIID::SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_HYDRAULICS_ELEC_2_SWITCH) && FSIcm::inst->get<float>(FSIID::SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE) > 50) {
				debug("HYDRAULICS ELEC 2 On");
                elec2_hyd.setPower(true);
			}
			else {
				debug("HYDRAULICS ELEC 2 Off");
                elec2_hyd.setPower(false);
			}
			sim_hydraulics();
		}


		//ENG 1
		if (id == FSIID::MBI_HYDRAULICS_ENG_1_SWITCH || id == FSIID::SLI_GEN_1_RTL)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_HYDRAULICS_ENG_1_SWITCH) && FSIcm::inst->get<bool>(FSIID::SLI_GEN_1_RTL)) {
				debug("HYDRAULICS ENG 1 On");
                eng1_hyd.setPower(true);
			}
			else {
				debug("HYDRAULICS ENG 1 Off");
                eng1_hyd.setPower(false);
			}

			sim_hydraulics();
		}

		//ENG 2
		if (id == FSIID::MBI_HYDRAULICS_ENG_2_SWITCH || id == FSIID::SLI_GEN_2_RTL)
		{
			if (FSIcm::inst->get<bool>(FSIID::MBI_HYDRAULICS_ENG_2_SWITCH) && FSIcm::inst->get<bool>(FSIID::SLI_GEN_2_RTL)) {
				debug("HYDRAULICS ENG 2 On");
                eng2_hyd.setPower(true);
			}
			else {
				debug("HYDRAULICS ENG 2 Off");
                eng2_hyd.setPower(false);
			}

			sim_hydraulics();
		}

        //Failures
        if (id == FSIID::MALFX_ELEC_HYD_PUMP_A_OVERHEAT_ACTIVE) {
            if (FSIcm::inst->get<bool>(FSIID::MALFX_ELEC_HYD_PUMP_A_OVERHEAT_ACTIVE)) {
                LightController::set(FSIID::MBI_HYDRAULICS_ELEC_2_OVERHEAT_LIGHT, true);
            } else {
                LightController::set(FSIID::MBI_HYDRAULICS_ELEC_2_OVERHEAT_LIGHT, false);
            }
            LightController::ProcessWrites();
        }
        if (id == FSIID::MALFX_ELEC_HYD_PUMP_B_OVERHEAT_ACTIVE) {
            if (FSIcm::inst->get<bool>(FSIID::MALFX_ELEC_HYD_PUMP_B_OVERHEAT_ACTIVE)) {
                LightController::set(FSIID::MBI_HYDRAULICS_ELEC_1_OVERHEAT_LIGHT, true);
            }
            else {
                LightController::set(FSIID::MBI_HYDRAULICS_ELEC_1_OVERHEAT_LIGHT, false);
            }
            LightController::ProcessWrites();
        }
	}


    void HYDRAULICS::sim_hydraulics_st() {
        instance->sim_hydraulics();
    }

	void HYDRAULICS::sim_hydraulics() {
		bool hyd_A_eng = false;
		bool hyd_B_eng = false;
		bool hyd_A_elec = false;
		bool hyd_B_elec = false;

		//gen 1 hyd pump
		if (eng1_hyd.pump_status & HYD_PUMP_STATUS::HYD_PUMP_PRESSURE) {
			LightController::set(FSIID::MBI_HYDRAULICS_ENG_1_LOW_PRESSURE_LIGHT, false);
			hyd_A_eng = true;
		}
		else {
			LightController::set(FSIID::MBI_HYDRAULICS_ENG_1_LOW_PRESSURE_LIGHT, true);
		}

		//gen 2 hyd pump
		if (eng2_hyd.pump_status & HYD_PUMP_STATUS::HYD_PUMP_PRESSURE) {
			LightController::set(FSIID::MBI_HYDRAULICS_ENG_2_LOW_PRESSURE_LIGHT, false);
			hyd_B_eng = true;
		}
		else {
			LightController::set(FSIID::MBI_HYDRAULICS_ENG_2_LOW_PRESSURE_LIGHT, true);
		}

		//elec 1 hyd pump
		if (elec1_hyd.pump_status & HYD_PUMP_STATUS::HYD_PUMP_PRESSURE)
		{
			hyd_B_elec = true;
			LightController::set(FSIID::MBI_HYDRAULICS_ELEC_1_LOW_PRESSURE_LIGHT, false);
		}
		else
		{
			LightController::set(FSIID::MBI_HYDRAULICS_ELEC_1_LOW_PRESSURE_LIGHT, true);
		}

		//elec 2 hyd pump
		if (elec2_hyd.pump_status & HYD_PUMP_STATUS::HYD_PUMP_PRESSURE)
		{
			hyd_A_elec = true;
			LightController::set(FSIID::MBI_HYDRAULICS_ELEC_2_LOW_PRESSURE_LIGHT, false);
		}
		else
		{
			LightController::set(FSIID::MBI_HYDRAULICS_ELEC_2_LOW_PRESSURE_LIGHT, true);
		}


		//system A pressure
		if (hyd_A_elec || hyd_A_eng)
		{
			FSIcm::inst->set<float>(FSIID::SLI_HYD_A_PRESSURE, 3000.0f);
		}
		else
		{
			FSIcm::inst->set<float>(FSIID::SLI_HYD_A_PRESSURE, 0);
		}

		//system B pressure
		if (hyd_B_elec || hyd_B_eng)
		{
			FSIcm::inst->set<float>(FSIID::SLI_HYD_B_PRESSURE, 3000.0f);
		}
		else
		{
			FSIcm::inst->set<float>(FSIID::SLI_HYD_B_PRESSURE, 0);
		}

		//stby hyd pressure when one elec xfr bus powered or engines are alive
		if (FSIcm::inst->get<float>(FSIID::SLI_AC_XFR_BUS_1_PHASE_1_VOLTAGE) > 50 || FSIcm::inst->get<float>(FSIID::SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE) > 50 || hyd_A_eng || hyd_B_eng)
		{
			FSIcm::inst->set<float>(FSIID::SLI_HYD_STBY_PRESSURE, 3000);
		}
		else
		{
			FSIcm::inst->set<float>(FSIID::SLI_HYD_STBY_PRESSURE, 0);
		}

		LightController::ProcessWrites();
	}

}