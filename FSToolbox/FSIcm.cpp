#include "FSIcm.h"


namespace fstoolbox {
	FSIcm * FSIcm::inst = nullptr;

	FSIcm::FSIcm(std::string desc) :
		fsinterface::FSIClient(desc)
	{
		inst = this;
	}

}

