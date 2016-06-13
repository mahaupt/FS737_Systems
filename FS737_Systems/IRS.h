#pragma once
//#include <iostream>

#include "Panel.h"
#include "FSToolbox/FSIcm.h"
#include "FSToolbox/LightController.h"
#include "FSToolbox/Timer.h"
#include "FSToolbox/TimerManager.h"

namespace fssystems
{
	using fstoolbox::FSIcm;
	using fsinterface::FSIID;
	using fstoolbox::LightController;
	using fstoolbox::Timer;
	using fstoolbox::TimerManager;
	using fstoolbox::TimeoutCallback;


	/*  ### FS737 IRS 0.1 ###
	SIMPLIFIED SIMULATION OF B737-800 IRS
	by Marcel Haupt and Arvid Preuss, 2016

	FOLLOWING CONDITIONS ARE SUPPOSED:
	* IRS Quick Alignment
	* NO FAILURES

	TODO:
	* Displays on ATT Mode
	* Failures
	* (Position Displacement)
	*/


    enum IRS_STATUS : int
    {
        IRS_OFFLINE =           0b00000001,
        IRS_ALIGNING =          0b00000010,
        IRS_ALIGNED =           0b00000100,
        IRS_FAULT =             0b00001000
    };
    enum IRS_POWER_STATUS : int
    {
        IRS_POWER_OFFLINE =     0b00000001,
        IRS_POWER_ON_DC =       0b00000010,
        IRS_POWER_AC_AVAILABLE= 0b00000100,
        IRS_POWER_ON_AC =       0b00001000,
        IRS_POWER_DC_FAULT =    0b00010000
    };


	//IRS Module
	class Irs_mod
	{
	private:
		Timer * alignmentStartTimer;
		Timer * alignTimer;
		Timer * dcOffTimer;

		static void alignOnCallback(void * inst);
		static void alignedCallback(void * inst);
		static void dcOffCallback(void * inst);

	public:
        int irs_status;
        int irs_power_status;

		Irs_mod();
		~Irs_mod();
		void setPowerStatus(bool value);
		void setACAvailable(bool value);
		bool isAligning();
	};


	class IRS :
		public Panel
	{
	private:
		Irs_mod irs_l;
		Irs_mod irs_r;
		static IRS * instance;

	public:
		IRS();

		static void fsiOnVarReceive(FSIID id);
		void onVarReceive(FSIID & id);
		static void st_sim_irs();
		void sim_irs();
	};

}