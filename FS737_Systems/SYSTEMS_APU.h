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

    enum APU_STATUS : int {
        APU_OFFLINE = 0b0000,
        APU_ONLINE = 0b0001,
        APU_SPOOLUP = 0b0010,
        APU_RUNNING = 0b0100,
        APU_READY_TO_LOAD = 0b1000 
    };


    class APU {
    public:
        int apu_status;
        float apu_egt;
        bool low_pressue;
        bool combustion;
        bool fuel_available;

        Timer online_to_spoolup_timer; //5s - low oil on
        Timer spoolup_to_egt_rising_timer;//16s apu EGT rising
                                        //20s low oil off, 400 °C
                                        //30s to 700 °C
                                        //slowly to 350 °C
        APU();
        void set_power(bool value);
        void start();
        void sim_apu(double time);
        static void spoolup_callback(void * inst);
        static void combustion_callback(void * inst);
    };


    class SYSTEMS_APU :
        public Panel
    {
    private:
        static SYSTEMS_APU * instance;
        APU apu;
    public:
        SYSTEMS_APU();
        static void fsiOnVarReceive(FSIID id);

        static void sim_apu_st();
        void sim_apu();
        static void timedFunction(double time);
    };
}
