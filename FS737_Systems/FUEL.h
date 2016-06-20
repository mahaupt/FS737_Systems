#pragma once
#include "Panel.h"
#include "FSToolbox/FSIcm.h"
#include "FSToolbox/LightController.h"
#include "FSToolbox/Hardware.h"


namespace fssystems
{
	using fstoolbox::FSIcm;
	using fsinterface::FSIID;
	using fstoolbox::LightController;

	class FUEL :
		public Panel
	{
	private:
		static FUEL * instance;
		VALVE valve_xfeed;
		PUMP left_forward, left_aft, center_left, center_right, right_forward, right_aft;
		PIPELINE pipeline_left, pipeline_right;
	public:
		FUEL();

		static void fsiOnVarReceive(FSIID id);
		void onVarReceive(FSIID & id);
		void run_all_machines();
	};	
}

