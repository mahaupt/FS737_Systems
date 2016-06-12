#pragma once
#include <iostream>
#include <string>

class Panel
{
public:
protected:
	bool is_debug = false;

	void debug(std::string str)
	{
		if (is_debug)
		{
			std::cout << str << std::endl;
		}
	}
};

