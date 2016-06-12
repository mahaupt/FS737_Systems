#include "FSIcm.h"


namespace fstoolbox {
	FSIcm::FSIcm(std::string desc) :
		fsinterface::FSIClient(desc)
	{
		inst = this;
	}

}

