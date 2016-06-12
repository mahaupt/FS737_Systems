#pragma once
#include "Panel.h"
#include "FSToolbox/FSIcm.h"
#include "FSToolbox/LightController.h"

namespace fssystems
{
	using fstoolbox::FSIcm;
	using fsinterface::FSIID;
	using fstoolbox::LightController;

	class OXY :
		public Panel
	{
	private:
		static OXY * instance;
	public:
		OXY();
		static void fsiOnVarReceive(FSIID id);
		void onVarReceive(FSIID & id);
	};

}

