#pragma once
#include "Panel.h"
#include "FSToolbox/FSIcm.h"
#include "FSToolbox/LightController.h"

namespace fssystems
{
	using fstoolbox::FSIcm;
	using fsinterface::FSIID;
	using fstoolbox::LightController;

	class ELT : public Panel
	{
	private:
		static ELT * instance;

	public:
		ELT();
		static void fsiOnVarReceive(FSIID id);
	};

}