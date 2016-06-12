#include "IRS.h"

namespace fssystems
{

	Irs_mod::Irs_mod()
	{
		alignmentStartTimer = new Timer(3.2, alignOnCallback);
		alignTimer = new Timer(60.0 * 3.0, alignedCallback);
		dcOffTimer = new Timer(3.0, dcOffCallback);

		TimerManager::addTimer(*alignmentStartTimer);
		TimerManager::addTimer(*alignTimer);
		TimerManager::addTimer(*dcOffTimer);
	}

	Irs_mod::~Irs_mod() {
		delete alignmentStartTimer;
		delete alignTimer;
		delete dcOffTimer;
	}

	void Irs_mod::setPowerStatus(bool value)
	{
		if (isOnline != value)
		{
			//set to online - start alignment
			if (value)
			{
				alignmentStartTimer->Start();

				if (acAvailable)
				{
					dcOffTimer->Start();
				}

				isOnline = true;
				onDC = true;
			}
			else //set offline
			{
				alignmentStartTimer->Reset();
				alignTimer->Reset();
				dcOffTimer->Reset();

				isAligned = false;
				isOnline = false;
			}
		}
	}

	void Irs_mod::setACAvailable(bool value)
	{
		if (acAvailable != value)
		{
			if (value)
			{
				if (isOnline)
				{
					dcOffTimer->Start();
				}
				acAvailable = true;
			}
			else
			{
				if (isOnline)
				{
					dcOffTimer->Reset();
				}
				onDC = true;
				acAvailable = false;
			}
		}
	}

	bool Irs_mod::isAligning()
	{
		return alignTimer->isEnabled();
	}


	TimeoutCallback Irs_mod::alignOnCallback()
	{
		alignTimer->Start();
		IRS::sim_irs();
	}

	TimeoutCallback Irs_mod::alignedCallback()
	{
		isAligned = true;
		IRS::sim_irs();
	}

	TimeoutCallback Irs_mod::dcOffCallback()
	{
		onDC = false;
		IRS::sim_irs();
	}

	IRS::IRS() :
		irs_l(), irs_r()
	{
		//debug variable
		is_debug = true;

		//starting FSI Client for IRS
		FSIcm.inst.OnVarReceiveEvent += fsiOnVarReceive;
		FSIID wanted_vars[] =
		{
			FSIID.SLI_BAT_BUS_VOLTAGE,
			FSIID.SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE,
			FSIID.SLI_AC_STBY_BUS_PHASE_1_VOLTAGE,

			FSIID.MBI_IRS_CONTROL_L_MODE_SWITCH_OFF_POS,
			FSIID.MBI_IRS_CONTROL_L_MODE_SWITCH_NAV_POS,
			FSIID.MBI_IRS_CONTROL_L_MODE_SWITCH_ATT_POS,
			FSIID.MBI_IRS_CONTROL_L_MODE_SWITCH_ALIGN_POS,
			FSIID.MBI_IRS_CONTROL_R_MODE_SWITCH_OFF_POS,
			FSIID.MBI_IRS_CONTROL_R_MODE_SWITCH_NAV_POS,
			FSIID.MBI_IRS_CONTROL_R_MODE_SWITCH_ATT_POS,
			FSIID.MBI_IRS_CONTROL_R_MODE_SWITCH_ALIGN_POS
		};
		FSIcm.inst.DeclareAsWanted(wanted_vars, sizeof(wanted_vars));

		//set Lights on the beginning
		LightController.set(FSIID.MBI_IRS_CONTROL_L_DC_FAIL_LIGHT, false);
		LightController.set(FSIID.MBI_IRS_CONTROL_L_ON_DC_LIGHT, false);
		LightController.set(FSIID.MBI_IRS_CONTROL_L_FAULT_LIGHT, false);
		LightController.set(FSIID.MBI_IRS_CONTROL_L_ALIGN_LIGHT, false);
		LightController.set(FSIID.MBI_IRS_CONTROL_R_DC_FAIL_LIGHT, false);
		LightController.set(FSIID.MBI_IRS_CONTROL_R_ON_DC_LIGHT, false);
		LightController.set(FSIID.MBI_IRS_CONTROL_R_FAULT_LIGHT, false);
		LightController.set(FSIID.MBI_IRS_CONTROL_R_ALIGN_LIGHT, false);

		FSIcm.inst.MBI_IRS_CONTROL_LAMPTEST = false;

		//send Settings to Server
		FSIcm.inst.ProcessWrites();
	}


	void fsiOnVarReceive(FSIID id)
	{

		//LEFT IRS KNOB or Power
		if (id == FSIID.MBI_IRS_CONTROL_L_MODE_SWITCH_OFF_POS ||
			id == FSIID.MBI_IRS_CONTROL_L_MODE_SWITCH_ALIGN_POS ||
			id == FSIID.MBI_IRS_CONTROL_L_MODE_SWITCH_NAV_POS ||
			id == FSIID.MBI_IRS_CONTROL_L_MODE_SWITCH_ATT_POS ||
			id == FSIID.SLI_BAT_BUS_VOLTAGE)
		{
			//switch to off if
			if (FSIcm.inst.MBI_IRS_CONTROL_L_MODE_SWITCH_OFF_POS || FSIcm.inst.SLI_BAT_BUS_VOLTAGE <= 12)
			{
				debug("IRS L OFF");

				irs_l.setPowerStatus(false);
				sim_irs();
			}
			else if (FSIcm.inst.MBI_IRS_CONTROL_L_MODE_SWITCH_ALIGN_POS)
			{
				debug("IRS L ALIGN");

				irs_l.setPowerStatus(true);
				sim_irs();
			}
			else if (FSIcm.inst.MBI_IRS_CONTROL_L_MODE_SWITCH_NAV_POS)
			{
				debug("IRS L NAV");

				irs_l.setPowerStatus(true);
				sim_irs();
			}
			else if (FSIcm.inst.MBI_IRS_CONTROL_L_MODE_SWITCH_ATT_POS)
			{
				debug("IRS L ATT");

				irs_l.setPowerStatus(true);
				sim_irs();
			}
		}


		//RIGHT IRS KNOB
		if (id == FSIID.MBI_IRS_CONTROL_R_MODE_SWITCH_OFF_POS ||
			id == FSIID.MBI_IRS_CONTROL_R_MODE_SWITCH_ALIGN_POS ||
			id == FSIID.MBI_IRS_CONTROL_R_MODE_SWITCH_NAV_POS ||
			id == FSIID.MBI_IRS_CONTROL_R_MODE_SWITCH_ATT_POS ||
			id == FSIID.SLI_BAT_BUS_VOLTAGE)
		{

			if (FSIcm.inst.MBI_IRS_CONTROL_R_MODE_SWITCH_OFF_POS || FSIcm.inst.SLI_BAT_BUS_VOLTAGE <= 12)
			{
				debug("IRS R OFF");

				irs_r.setPowerStatus(false);
				sim_irs();
			}
			else if (FSIcm.inst.MBI_IRS_CONTROL_R_MODE_SWITCH_ALIGN_POS)
			{
				debug("IRS R ALIGN");

				irs_r.setPowerStatus(true);
				sim_irs();
			}
			else if (FSIcm.inst.MBI_IRS_CONTROL_R_MODE_SWITCH_NAV_POS)
			{
				debug("IRS R NAV");

				irs_r.setPowerStatus(true);
				sim_irs();
			}
			else if (FSIcm.inst.MBI_IRS_CONTROL_R_MODE_SWITCH_ATT_POS)
			{
				debug("IRS R ATT");

				irs_r.setPowerStatus(true);
				sim_irs();
			}
		}


		//AC Powersources
		if (id == FSIID.SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE)
		{
			//no voltage on XFR BUS 2
			if (FSIcm.inst.SLI_AC_XFR_BUS_2_PHASE_1_VOLTAGE <= 50)
			{
				irs_r.setACAvailable(false);
			}
			else
			{
				irs_r.setACAvailable(true);
			}
			sim_irs();
		}

		//läuft eigentlich über stby bus
		if (id == FSIID.SLI_AC_STBY_BUS_PHASE_1_VOLTAGE)
		{
			if (FSIcm.inst.SLI_AC_STBY_BUS_PHASE_1_VOLTAGE <= 50)
			{
				irs_l.setACAvailable(false);
			}
			else
			{
				irs_l.setACAvailable(true);
			}
			sim_irs();
		}
	}


	public static void sim_irs()
	{
		//IRS L ALIGN LIGHT
		if (irs_l.isOnline && irs_l.isAligning())
		{
			LightController.set(FSIID.MBI_IRS_CONTROL_L_ALIGN_LIGHT, true);
		}
		else
		{
			LightController.set(FSIID.MBI_IRS_CONTROL_L_ALIGN_LIGHT, false);
		}

		//IRS R ALIGN LIGHT
		if (irs_r.isOnline && irs_r.isAligning())
		{
			LightController.set(FSIID.MBI_IRS_CONTROL_R_ALIGN_LIGHT, true);
		}
		else
		{
			LightController.set(FSIID.MBI_IRS_CONTROL_R_ALIGN_LIGHT, false);
		}

		//IRS L ON DC
		if (irs_l.isOnline && irs_l.onDC)
		{
			LightController.set(FSIID.MBI_IRS_CONTROL_L_ON_DC_LIGHT, true);
		}
		else
		{
			LightController.set(FSIID.MBI_IRS_CONTROL_L_ON_DC_LIGHT, false);
		}

		//IRS R ON DC
		if (irs_r.isOnline && irs_r.onDC)
		{
			LightController.set(FSIID.MBI_IRS_CONTROL_R_ON_DC_LIGHT, true);
		}
		else
		{
			LightController.set(FSIID.MBI_IRS_CONTROL_R_ON_DC_LIGHT, false);
		}

		LightController.ProcessWrites();
	}


}