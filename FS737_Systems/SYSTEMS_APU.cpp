#include "SYSTEMS_APU.h"



namespace fssystems
{
    APU::APU() :
        online_to_spoolup_timer(5, spoolup_callback, this),
        spoolup_to_egt_rising_timer(10, combustion_callback, this)
    {
        apu_status = APU_STATUS::APU_OFFLINE;
        low_pressue = false;
        combustion = false;
        fuel_available = true;
        apu_egt = 0;

        TimerManager::addTimer(online_to_spoolup_timer);
        TimerManager::addTimer(spoolup_to_egt_rising_timer);
    }

    void APU::set_power(bool value) {
        if (value != (apu_status & APU_STATUS::APU_ONLINE)) {
            if (value) {
                //APU to Online
                apu_status |= APU_STATUS::APU_ONLINE;
            } else {
                //APU to Offline
                apu_status &= ~APU_STATUS::APU_ONLINE;
                apu_status &= ~APU_STATUS::APU_READY_TO_LOAD;
                apu_status &= ~APU_STATUS::APU_RUNNING;
                apu_status &= ~APU_STATUS::APU_SPOOLUP;

                low_pressue = false;
                combustion = false;
            }
        }
    }

    void APU::start() {
        if (!(apu_status & APU_STATUS::APU_SPOOLUP)) {
            online_to_spoolup_timer.Start();
        }
    }

    void APU::spoolup_callback(void * inst) {
        ((APU*)inst)->apu_status |= APU_STATUS::APU_SPOOLUP;
        ((APU*)inst)->low_pressue = true;
        ((APU*)inst)->spoolup_to_egt_rising_timer.Start();
        SYSTEMS_APU::sim_apu_st();
    }

    void APU::combustion_callback(void * inst) {
        ((APU*)inst)->combustion = true;
    }

    void APU::sim_apu(double time) {
        float egt_soll = 0;
        if ((apu_status & APU_STATUS::APU_ONLINE) && combustion && fuel_available) {
            if (apu_status & APU_STATUS::APU_SPOOLUP) {
                egt_soll = 750;
                //oil pressure
                if(apu_egt > 380 && low_pressue) {
                    low_pressue = false;
                    SYSTEMS_APU::sim_apu_st();
                }

                //spoolup end
                if (apu_egt > 700) {
                    apu_status &= ~APU_STATUS::APU_SPOOLUP;
                    apu_status |= APU_STATUS::APU_RUNNING;
                }
            } else if (apu_status & APU_STATUS::APU_RUNNING) {
                egt_soll = 320;

                if (apu_egt < 360 && !(apu_status & APU_STATUS::APU_READY_TO_LOAD)) {
                    apu_status |= APU_STATUS::APU_READY_TO_LOAD;
                    SYSTEMS_APU::sim_apu_st();
                }
            }
        }

        //egt to egt_soll
        apu_egt += (egt_soll - apu_egt) / 8 * (float)time;
        //std::cout << apu_egt << std::endl;
    }

    SYSTEMS_APU * SYSTEMS_APU::instance;

    SYSTEMS_APU::SYSTEMS_APU()
    {
        //debug variable
        is_debug = true;

        instance = this;

        //starting FSI Client for IRS
        FSIcm::inst->RegisterCallback(fsiOnVarReceive);
        FSIID wanted_vars[] =
        {
            FSIID::MBI_LOWER_T_BOTTOM_APU_SWITCH_ON_POS,
            FSIID::MBI_LOWER_T_BOTTOM_APU_SWITCH_START_POS,
            FSIID::SLI_FUEL_APU_AVAIL
        };
        FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));

        //standard values
        LightController::registerLight(FSIID::MBI_ELEC_BUS_APU_FAULT_LIGHT);
        LightController::registerLight(FSIID::MBI_ELEC_BUS_APU_LOW_OIL_PRESSURE_LIGHT);
        LightController::registerLight(FSIID::MBI_ELEC_BUS_APU_MAINT_LIGHT);
        LightController::registerLight(FSIID::MBI_ELEC_BUS_APU_OVERSPEED_LIGHT);

        FSIcm::inst->ProcessWrites();


        TimerManager::addTimedCallback(timedFunction);
    }


    void SYSTEMS_APU::fsiOnVarReceive(FSIID id) {
        //FUEL AVAILABLE
        if (id == FSIID::SLI_FUEL_APU_AVAIL) {
            if (FSIcm::inst->get<bool>(FSIID::SLI_FUEL_APU_AVAIL)) {
                instance->apu.fuel_available = true;
            } else {
                instance->apu.fuel_available = false;
            }
        }

        //APU Online and start
        if (id == FSIID::MBI_LOWER_T_BOTTOM_APU_SWITCH_ON_POS || id == FSIID::MBI_LOWER_T_BOTTOM_APU_SWITCH_START_POS) {
            bool apu_on = FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_BOTTOM_APU_SWITCH_ON_POS);
            bool apu_start = FSIcm::inst->get<bool>(FSIID::MBI_LOWER_T_BOTTOM_APU_SWITCH_START_POS);

            if (apu_on || apu_start) {
                instance->apu.set_power(true);
                instance->debug("APU On");
                sim_apu_st();
            } else {
                instance->apu.set_power(false);
                instance->debug("APU Off");
                sim_apu_st();
            }

            if (apu_start) {
                instance->debug("APU Start");
                instance->apu.start();
            }
        }
    }

    void SYSTEMS_APU::sim_apu_st() {
        instance->sim_apu();
    }

    void SYSTEMS_APU::sim_apu() {
        //apu low oil pressure
        if (apu.low_pressue) {
            LightController::set(FSIID::MBI_ELEC_BUS_APU_LOW_OIL_PRESSURE_LIGHT, true);
        } else {
            LightController::set(FSIID::MBI_ELEC_BUS_APU_LOW_OIL_PRESSURE_LIGHT, false);
        }

        //apu ready to load
        if (apu.apu_status & APU_STATUS::APU_READY_TO_LOAD) {
            FSIcm::inst->set<bool>(FSIID::SLI_APU_GEN_RTL, true);
        } else {
            FSIcm::inst->set<bool>(FSIID::SLI_APU_GEN_RTL, false);
        }

        LightController::ProcessWrites();
    }


    void SYSTEMS_APU::timedFunction(double time) {
        instance->apu.sim_apu(time);
    }
}