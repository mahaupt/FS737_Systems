#pragma once
#include "Panel.h"
#include "FSToolbox/FSIcm.h"
#include "FSToolbox/LightController.h"

namespace fssystems
{
	using fstoolbox::FSIcm;
	using fsinterface::FSIID;
	using fstoolbox::LightController;

	class HEAT :
		public Panel
	{
	private:
		static HEAT * instance;
	public:
		HEAT();
		static void fsiOnVarReceive(FSIID id);
		void onVarReceive(FSIID & id);
	};

}