#pragma once
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


    enum HYD_PUMP_STATUS : int {
        HYD_PUMP_ONLINE = 0b01,
        HYD_PUMP_PRESSURE = 0b10
    };
    class HYD_Pump {
    private:
        Timer timer_pressurize;
        Timer timer_depressurize;
    public:
        int pump_status;

        HYD_Pump();
        void setPower(bool value);
        static void callback_pressurize(void * inst);
        static void callback_depressurize(void * inst);
    };

	class HYDRAULICS :
		public Panel
	{
	private:
		static HYDRAULICS * instance;
        HYD_Pump eng1_hyd, eng2_hyd, elec1_hyd, elec2_hyd;
	public:
		HYDRAULICS();
		
		static void fsiOnVarReceive(FSIID id);
		void onVarReceive(FSIID & id);
        static void sim_hydraulics_st();
		void sim_hydraulics();
	};

}