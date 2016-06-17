#pragma once
#include "Panel.h"
#include "FSToolbox/FSIcm.h"
#include "FSToolbox/LightController.h"

namespace fssystems 
{
    using fstoolbox::FSIcm;
    using fsinterface::FSIID;
    using fstoolbox::LightController;

    class SYSTEMS_Gear :
        public Panel
    {
    private:
        static SYSTEMS_Gear * instance;
    public:
        SYSTEMS_Gear();
        static void fsiOnVarReceive(FSIID id);
    };

}