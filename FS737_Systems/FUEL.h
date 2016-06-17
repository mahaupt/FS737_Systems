#pragma once
#include "Panel.h"
#include "FSToolbox/FSIcm.h"
#include "FSToolbox/LightController.h"
#include "FSToolbox/TimerManager.h"
#include "FSToolbox/Timer.h"

namespace fssystems
{
	using fstoolbox::FSIcm;
	using fsinterface::FSIID;
	using fstoolbox::LightController;
    using fstoolbox::Timer;
    using fstoolbox::TimerManager;

    enum FUEL_PMP_STAT : int {
        PUMP_OFFLINE = 0b0000,
        PUMP_ONLINE = 0b0001,
        PUMP_PRESSURE = 0b0010,
        PUMP_NO_FUEL_CUTOUT = 0b0100
    };
    class FuelPump {
        private:
            Timer pressurize_timer, depressurize_timer;
            float fuel_level;
            
        public:
            int pump_status;

            FuelPump();
            void setPower(bool value);
            void setFuelLevel(float value);
            static void pressurizeCallback(void * instance);
            static void depressurizeCallback(void * instance);
    };

	class FUEL :
		public Panel
	{
	private:
		static FUEL * instance;

        FuelPump pmp_ctr_l, pmp_ctr_r, pmp_l_aft, pmp_l_fwd, pmp_r_aft, pmp_r_fwd;
	public:
		FUEL();

		static void fsiOnVarReceive(FSIID id);
		void onVarReceive(FSIID & id);
        static void sim_fuel_st();
        void sim_fuel();
	};
}

