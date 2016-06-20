#pragma once
#include <iostream>
#include <string>

typedef unsigned char byte;

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
    
    unsigned int saturate(unsigned int value, unsigned int min, unsigned int max) {
        if (value > max)
            value = max;
        if (value < min)
            value = min;
        return value;
    }
    
    byte saturate(byte value, byte min, byte max) {
        if (value > max)
            value = max;
        if (value < min)
            value = min;
        return value;
    }
};

