#pragma once
#include "Panel.h"
#include "FSToolbox/FSIcm.h"
#include "FSToolbox/LightController.h"

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
	public:
		FUEL();

		static void fsiOnVarReceive(FSIID id);
		void onVarReceive(FSIID & id);
	};
}

