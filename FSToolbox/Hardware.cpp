#include "Hardware.h"

namespace fssystems
{
	VALVE::VALVE()
	{
		valvestate actual_state = sSHUT;
	}
	void VALVE::run_machine(bool open_command)
	{
		while (!state_finalized)
		{
			state_finalized = false;
			switch (actual_state) {
			case sCLOSING:
				if (open_command) actual_state = sOPENING;
				else actual_state = sSHUT;
				break;
			case sSHUT:
				if (open_command) actual_state = sOPENING;
				else state_finalized;
				break;
			case sOPEN:
				if (!open_command) actual_state = sCLOSING;
				else state_finalized;
				break;
			case sOPENING:
				if (!open_command) actual_state = sCLOSING;
				else actual_state = sOPEN;
				break;
			}
		}
	}
	
	void PIPELINE::setstate(pipelinestate new_state)
	{
		actual_state = new_state;
	}

	PUMP::PUMP()
	{
		pumpstate actual_state = sOFF;
	}
	void PUMP::run_machine(bool _switch, bool power, bool fail)
	{
		while (!state_finalized)
		{
			state_finalized = false;
			switch (actual_state) {
			case sON:
				if (!_switch || !power) actual_state = sOFF;
				if (fail) actual_state = sFAIL;
				state_finalized = true;
				break;
			case sOFF:
				if (fail) actual_state = sFAIL;
				if (_switch && power) actual_state = sON;
				state_finalized = true;
				break;
			case sFAIL:
				state_finalized = true;
				break;
			}
		}
	}
	void PUMP::setstate(pumpstate new_state)
	{
		actual_state = new_state;
	}

	PIPELINE::PIPELINE()
	{
		actual_state = sSHUTOFF;
	}
	void PIPELINE::run_machine(pumpstate ownpumps[], pumpstate foreignpumps[], valvestate xfeed, valvestate shutoff)
	{
		bool pump_available = false;
		pipelinestate old_state;
		for (int i = 0; i = 2; i++)
		{
			if (ownpumps[i] == sON || (foreignpumps[i] == sON && xfeed)) pump_available = true;
		}
		while (actual_state != old_state)
		{			
			old_state = actual_state;
			switch (actual_state) {
			case sSHUTOFF:
				if (shutoff == sSHUT) actual_state = sSHUTOFF;
				else if (pump_available) actual_state = sPRESSURIZED;
				else actual_state = sUNPRESSURIZED;
				break;
			case sUNPRESSURIZED:
				if (shutoff == sSHUT) actual_state = sSHUTOFF;
				else if (pump_available) actual_state = sPRESSURIZED;
				break;
			case sPRESSURIZED:				
				if (shutoff == sSHUT) actual_state = sSHUTOFF;
				else if (!pump_available) actual_state = sUNPRESSURIZED;
				break;
			}
		}
	}
	void PIPELINE::setstate(pipelinestate new_state)
	{
		actual_state = new_state;
	}
}