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
		bool isOnline = false;
		bool isAligned = false;
		bool acAvailable = true;
		bool onDC = false;

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