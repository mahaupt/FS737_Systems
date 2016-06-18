#include "IRS.h"

namespace fssystems
{

	Irs_mod::Irs_mod()
	{
		alignmentStartTimer = new Timer(3.2, alignOnCallback, (void*)this);
		alignTimer = new Timer(60.0 * 3.0, alignedCallback,(void*) this);
		dcOffTimer = new Timer(3.0, dcOffCallback, (void*)this);

		TimerManager::addTimer(*alignmentStartTimer);
		TimerManager::addTimer(*alignTimer);
		TimerManager::addTimer(*dcOffTimer);

        irs_status = IRS_STATUS::IRS_OFFLINE;
        irs_power_status = IRS_POWER_STATUS::IRS_POWER_OFFLINE;
	}

	Irs_mod::~Irs_mod() {
		delete alignmentStartTimer;
		delete alignTimer;
		delete dcOffTimer;
	}

	void Irs_mod::setPowerStatus(bool value)
	{
        //value is diffrernt from current online status
		if (!(irs_status & IRS_STATUS::IRS_OFFLINE) != value)
		{
			//set to online - start alignment
			if (value)
			{
				alignmentStartTimer->Start();

                //AC Power available
				if (irs_power_status & IRS_POWER_STATUS::IRS_POWER_AC_AVAILABLE)
				{
					dcOffTimer->Start();
				}

                //disable offline status
                irs_status &= ~IRS_STATUS::IRS_OFFLINE;
                irs_power_status &= ~IRS_POWER_STATUS::IRS_POWER_OFFLINE;

                //enable DC Power
                irs_power_status |= IRS_POWER_STATUS::IRS_POWER_ON_DC;

                //no AC yet
                irs_power_status &= ~IRS_POWER_STATUS::IRS_POWER_ON_AC;
			}
			else //set offline
			{
				alignmentStartTimer->Reset();
				alignTimer->Reset();
				dcOffTimer->Reset();

                irs_status |= IRS_STATUS::IRS_OFFLINE;
                irs_status &= ~(IRS_STATUS::IRS_ALIGNING | IRS_STATUS::IRS_ALIGNED);
                irs_power_status |= IRS_POWER_STATUS::IRS_POWER_OFFLINE;
                irs_power_status &= ~(IRS_POWER_STATUS::IRS_POWER_ON_AC | IRS_POWER_STATUS::IRS_POWER_ON_DC);
			}
		}
	}

	void Irs_mod::setACAvailable(bool value)
	{
        if (((irs_power_status & IRS_POWER_STATUS::IRS_POWER_AC_AVAILABLE) != 0) != value)
        {
		    if (value)
		    {
                if (!(irs_power_status & IRS_POWER_STATUS::IRS_POWER_ON_AC)) 
                {//irs not on AC yet
                    if (!(irs_status & IRS_STATUS::IRS_OFFLINE))
                    { //IRS ONLINE
				        dcOffTimer->Start();
			        }
                }
                irs_power_status |= IRS_POWER_STATUS::IRS_POWER_AC_AVAILABLE;
		    }
		    else
		    {
			    if (!(irs_status & IRS_STATUS::IRS_OFFLINE))
			    { //IRS ONLINE
				    dcOffTimer->Reset();
			    }
                irs_power_status |= IRS_POWER_STATUS::IRS_POWER_ON_DC;
                irs_power_status &= ~(IRS_POWER_STATUS::IRS_POWER_AC_AVAILABLE | IRS_POWER_STATUS::IRS_POWER_ON_AC);
		    }
        }
	}

	bool Irs_mod::isAligning()
	{
		return alignTimer->isEnabled();
	}


	void Irs_mod::alignOnCallback(void * inst)
	{
		//std::cout << 1 << std::endl;
        ((Irs_mod*)inst)->irs_status |= IRS_STATUS::IRS_ALIGNING;
		((Irs_mod*)inst)->alignTimer->Start();
		IRS::st_sim_irs();
	}

	void Irs_mod::alignedCallback(void * inst)
	{
		//std::cout << 2 << std::endl;
        ((Irs_mod*)inst)->irs_status &= ~IRS_STATUS::IRS_ALIGNING;
		((Irs_mod*)inst)->irs_status |= IRS_STATUS::IRS_ALIGNED;
		IRS::st_sim_irs();
	}

    //switch on ac
	void Irs_mod::dcOffCallback(void * inst)
	{
		//std::cout << 3 << std::endl;
		((Irs_mod*)inst)->irs_power_status |= IRS_POWER_STATUS::IRS_POWER_ON_AC;
        ((Irs_mod*)inst)->irs_power_status &= ~IRS_POWER_STATUS::IRS_POWER_ON_DC;
		IRS::st_sim_irs();
	}


	IRS * IRS::instance = nullptr;

	IRS::IRS()
	{
		//debug variable
		is_debug = true;

		instance = this;

		//starting FSI Client for IRS
		FSIcm::inst->RegisterCallback(fsiOnVarReceive);
		FSIID wanted_vars[] =
		{
			FSIID::SLI_BAT_BUS_VOLTAGE,
			FSIID::SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE,
			FSIID::SLI_AC_STBY_BUS_PHASE_1_VOLTAGE,

			FSIID::MBI_IRS_CONTROL_L_MODE_SWITCH_OFF_POS,
			FSIID::MBI_IRS_CONTROL_L_MODE_SWITCH_NAV_POS,
			FSIID::MBI_IRS_CONTROL_L_MODE_SWITCH_ATT_POS,
			FSIID::MBI_IRS_CONTROL_L_MODE_SWITCH_ALIGN_POS,
			FSIID::MBI_IRS_CONTROL_R_MODE_SWITCH_OFF_POS,
			FSIID::MBI_IRS_CONTROL_R_MODE_SWITCH_NAV_POS,
			FSIID::MBI_IRS_CONTROL_R_MODE_SWITCH_ATT_POS,
			FSIID::MBI_IRS_CONTROL_R_MODE_SWITCH_ALIGN_POS,

            //FAILURES
            FSIID::MALFX_IRS_L_FAIL_ACTIVE,
            FSIID::MALFX_IRS_R_FAIL_ACTIVE
		};
		FSIcm::inst->DeclareAsWanted(wanted_vars, sizeof(wanted_vars));

		//set Lights on the beginning
		LightController::registerLight(FSIID::MBI_IRS_CONTROL_L_DC_FAIL_LIGHT);
		LightController::registerLight(FSIID::MBI_IRS_CONTROL_L_ON_DC_LIGHT);
		LightController::registerLight(FSIID::MBI_IRS_CONTROL_L_FAULT_LIGHT);
		LightController::registerLight(FSIID::MBI_IRS_CONTROL_L_ALIGN_LIGHT);
		LightController::registerLight(FSIID::MBI_IRS_CONTROL_R_DC_FAIL_LIGHT);
		LightController::registerLight(FSIID::MBI_IRS_CONTROL_R_ON_DC_LIGHT);
		LightController::registerLight(FSIID::MBI_IRS_CONTROL_R_FAULT_LIGHT);
		LightController::registerLight(FSIID::MBI_IRS_CONTROL_R_ALIGN_LIGHT);

		FSIcm::inst->set<bool>(FSIID::MBI_IRS_CONTROL_LAMPTEST, false);

		//send Settings to Server
		FSIcm::inst->ProcessWrites();
	}

	void IRS::fsiOnVarReceive(FSIID id) {
		instance->onVarReceive(id);
	}

	void IRS::onVarReceive(FSIID & id)
	{

		//LEFT IRS KNOB or Power
		if (id == FSIID::MBI_IRS_CONTROL_L_MODE_SWITCH_OFF_POS ||
			id == FSIID::MBI_IRS_CONTROL_L_MODE_SWITCH_ALIGN_POS ||
			id == FSIID::MBI_IRS_CONTROL_L_MODE_SWITCH_NAV_POS ||
			id == FSIID::MBI_IRS_CONTROL_L_MODE_SWITCH_ATT_POS ||
			id == FSIID::SLI_BAT_BUS_VOLTAGE)
		{
			//switch to off if
			if (FSIcm::inst->get<bool>(FSIID::MBI_IRS_CONTROL_L_MODE_SWITCH_OFF_POS) || FSIcm::inst->get<float>(FSIID::SLI_BAT_BUS_VOLTAGE) <= 12)
			{
				debug("IRS L OFF");

				irs_l.setPowerStatus(false);
				sim_irs();
			}
			else if (FSIcm::inst->get<bool>(FSIID::MBI_IRS_CONTROL_L_MODE_SWITCH_ALIGN_POS))
			{
				debug("IRS L ALIGN");

				irs_l.setPowerStatus(true);
				sim_irs();
			}
			else if (FSIcm::inst->get<bool>(FSIID::MBI_IRS_CONTROL_L_MODE_SWITCH_NAV_POS))
			{
				debug("IRS L NAV");

				irs_l.setPowerStatus(true);
				sim_irs();
			}
			else if (FSIcm::inst->get<bool>(FSIID::MBI_IRS_CONTROL_L_MODE_SWITCH_ATT_POS))
			{
				debug("IRS L ATT");

				irs_l.setPowerStatus(true);
				sim_irs();
			}
		}


		//RIGHT IRS KNOB
		if (id == FSIID::MBI_IRS_CONTROL_R_MODE_SWITCH_OFF_POS ||
			id == FSIID::MBI_IRS_CONTROL_R_MODE_SWITCH_ALIGN_POS ||
			id == FSIID::MBI_IRS_CONTROL_R_MODE_SWITCH_NAV_POS ||
			id == FSIID::MBI_IRS_CONTROL_R_MODE_SWITCH_ATT_POS ||
			id == FSIID::SLI_BAT_BUS_VOLTAGE)
		{

			if (FSIcm::inst->get<bool>(FSIID::MBI_IRS_CONTROL_R_MODE_SWITCH_OFF_POS) || FSIcm::inst->get<float>(FSIID::SLI_BAT_BUS_VOLTAGE) <= 12)
			{
				debug("IRS R OFF");

				irs_r.setPowerStatus(false);
				sim_irs();
			}
			else if (FSIcm::inst->get<bool>(FSIID::MBI_IRS_CONTROL_R_MODE_SWITCH_ALIGN_POS))
			{
				debug("IRS R ALIGN");

				irs_r.setPowerStatus(true);
				sim_irs();
			}
			else if (FSIcm::inst->get<bool>(FSIID::MBI_IRS_CONTROL_R_MODE_SWITCH_NAV_POS))
			{
				debug("IRS R NAV");

				irs_r.setPowerStatus(true);
				sim_irs();
			}
			else if (FSIcm::inst->get<bool>(FSIID::MBI_IRS_CONTROL_R_MODE_SWITCH_ATT_POS))
			{
				debug("IRS R ATT");

				irs_r.setPowerStatus(true);
				sim_irs();
			}
		}


		//AC Powersources
		if (id == FSIID::SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE)
		{
			//no voltage on XFR BUS 2
			if (FSIcm::inst->get<float>(FSIID::SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE) <= 50)
			{
				irs_r.setACAvailable(false);
			}
			else
			{
				irs_r.setACAvailable(true);
			}
			sim_irs();
		}

		//AC Powersources
		if (id == FSIID::SLI_AC_STBY_BUS_PHASE_1_VOLTAGE)
		{
			if (FSIcm::inst->get<float>(FSIID::SLI_AC_STBY_BUS_PHASE_1_VOLTAGE) <= 50)
			{
				irs_l.setACAvailable(false);
			}
			else
			{
				irs_l.setACAvailable(true);
			}
			sim_irs();
		}

        //failures
        if (id == FSIID::MALFX_IRS_L_FAIL_ACTIVE) {
            if (FSIcm::inst->get<bool>(FSIID::MALFX_IRS_L_FAIL_ACTIVE)) {
                irs_l.irs_status |= IRS_STATUS::IRS_FAULT;
            }
            else {
                irs_l.irs_status &= ~IRS_STATUS::IRS_FAULT;
            }
            sim_irs();
        }

        //failures
        if (id == FSIID::MALFX_IRS_R_FAIL_ACTIVE) {
            if (FSIcm::inst->get<bool>(FSIID::MALFX_IRS_R_FAIL_ACTIVE)) {
                irs_r.irs_status |= IRS_STATUS::IRS_FAULT;
            }
            else {
                irs_r.irs_status &= ~IRS_STATUS::IRS_FAULT;
            }
            sim_irs();
        }
	}


	void IRS::st_sim_irs() {
		instance->sim_irs();
	}


	void IRS::sim_irs()
	{
        bool irs_l_online = !(irs_l.irs_status & IRS_STATUS::IRS_OFFLINE);
        bool irs_r_online = !(irs_r.irs_status & IRS_STATUS::IRS_OFFLINE);

		//IRS L ALIGN LIGHT
		if (irs_l_online && irs_l.irs_status & IRS_STATUS::IRS_ALIGNING)
		{
			LightController::set(FSIID::MBI_IRS_CONTROL_L_ALIGN_LIGHT, true);
		}
		else
		{
			LightController::set(FSIID::MBI_IRS_CONTROL_L_ALIGN_LIGHT, false);
		}

		//IRS R ALIGN LIGHT
		if (irs_r_online && irs_r.irs_status & IRS_STATUS::IRS_ALIGNING)
		{
			LightController::set(FSIID::MBI_IRS_CONTROL_R_ALIGN_LIGHT, true);
		}
		else
		{
			LightController::set(FSIID::MBI_IRS_CONTROL_R_ALIGN_LIGHT, false);
		}

		//IRS L ON DC
		if (irs_l_online && irs_l.irs_power_status & IRS_POWER_STATUS::IRS_POWER_ON_DC)
		{
			LightController::set(FSIID::MBI_IRS_CONTROL_L_ON_DC_LIGHT, true);
		}
		else
		{
			LightController::set(FSIID::MBI_IRS_CONTROL_L_ON_DC_LIGHT, false);
		}

		//IRS R ON DC
		if (irs_r_online && irs_r.irs_power_status & IRS_POWER_STATUS::IRS_POWER_ON_DC)
		{
			LightController::set(FSIID::MBI_IRS_CONTROL_R_ON_DC_LIGHT, true);
		}
		else
		{
			LightController::set(FSIID::MBI_IRS_CONTROL_R_ON_DC_LIGHT, false);
		}

        //IRS L DC FAULT
        if (irs_l_online && irs_l.irs_power_status & IRS_POWER_STATUS::IRS_POWER_DC_FAULT)
        {
            LightController::set(FSIID::MBI_IRS_CONTROL_L_DC_FAIL_LIGHT, true);
        }
        else
        {
            LightController::set(FSIID::MBI_IRS_CONTROL_L_DC_FAIL_LIGHT, false);
        }

        //IRS R DC FAULT
        if (irs_r_online && irs_r.irs_power_status & IRS_POWER_STATUS::IRS_POWER_DC_FAULT)
        {
            LightController::set(FSIID::MBI_IRS_CONTROL_R_DC_FAIL_LIGHT, true);
        }
        else
        {
            LightController::set(FSIID::MBI_IRS_CONTROL_R_DC_FAIL_LIGHT, false);
        }

        //IRS L FAULT
        if (irs_l_online && irs_l.irs_status & IRS_STATUS::IRS_FAULT)
        {
            LightController::set(FSIID::MBI_IRS_CONTROL_L_FAULT_LIGHT, true);
        }
        else
        {
            LightController::set(FSIID::MBI_IRS_CONTROL_L_FAULT_LIGHT, false);
        }

        //IRS R FAULT
        if (irs_r_online && irs_r.irs_status & IRS_STATUS::IRS_FAULT)
        {
            LightController::set(FSIID::MBI_IRS_CONTROL_R_FAULT_LIGHT, true);
        }
        else
        {
            LightController::set(FSIID::MBI_IRS_CONTROL_R_FAULT_LIGHT, false);
        }

		LightController::ProcessWrites();
	}


}