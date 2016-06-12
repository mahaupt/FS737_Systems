#pragma once
#include "Panel.h"
#include "FSToolbox/FSIcm.h"
#include "FSToolbox/LightController.h"

namespace fssystems
{
	using fstoolbox::FSIcm;
	using fsinterface::FSIID;
	using fstoolbox::LightController;

	class TPANEL :
		public Panel
	{
	private:
		static TPANEL * instance;
	public:
		TPANEL();
		static void fsiOnVarReceive(FSIID id);
		void onVarReceive(FSIID & id);
	};
}

