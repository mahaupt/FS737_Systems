#pragma once
#include "Panel.h"
#include "FSToolbox/FSIcm.h"
#include "FSToolbox/LightController.h"

namespace fssystems
{
	using fstoolbox::FSIcm;
	using fsinterface::FSIID;
	using fstoolbox::LightController;

	class HYDRAULICS :
		public Panel
	{
	private:
		static HYDRAULICS * instance;
	public:
		HYDRAULICS();
		
		static void fsiOnVarReceive(FSIID id);
		void onVarReceive(FSIID & id);
		void sim_hydraulics();
	};

}