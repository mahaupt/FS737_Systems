#pragma once
#include "Panel.h"
#include "FSToolbox/FSIcm.h"
#include "FSToolbox/LightController.h"
#include "FSToolbox/TimerManager.h"

namespace fssystems
{

	using fstoolbox::FSIcm;
	using fsinterface::FSIID;
	using fstoolbox::LightController;
    using fstoolbox::TimerManager;

	class LED :
		public Panel
	{
	private:
		static LED * instance;
        
        
	public:
		LED();
		static void fsiOnVarReceive(FSIID id);
		void onVarReceive(FSIID & id);
        
        static void timedFunction(double time);
	};

}

