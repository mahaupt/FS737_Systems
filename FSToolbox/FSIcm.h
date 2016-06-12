#pragma once
#include <string>

#include "FSInterface/FSIClient.h"



namespace fstoolbox
{


	class FSIcm : public fsinterface::FSIClient
	{
	public:
		static FSIcm * inst;

		FSIcm(std::string desc);
	};

}