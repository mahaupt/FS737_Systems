#pragma once
#include "Panel.h"
#include "FSToolbox/FSIcm.h"
#include "FSToolbox/LightController.h"

namespace fssystems
{
	using fstoolbox::FSIcm;
	using fsinterface::FSIID;
	using fstoolbox::LightController;

	class DOORS :
		public Panel
	{
	public:
		DOORS();
		static void fsiOnVarReceive(FSIID id);
	};

}

