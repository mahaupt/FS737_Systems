#pragma once

namespace fssystems
{
	typedef enum { sOPEN = 1, sSHUT, sOPENING, sCLOSING } valvestate;
	typedef enum { sON = 1, sOFF, sFAIL } pumpstate;
	typedef enum { sPRESSURIZED = 1, sUNPRESSURIZED, sSHUTOFF } pipelinestate;

	class VALVE
	{		
	private:
		bool state_finalized = false;
	public:
		VALVE();
		valvestate actual_state;
		void run_machine(bool open_command);
		void setstate(valvestate new_state);
	};

	class PUMP
	{
	private:
		bool state_finalized = false;
	public:		
		PUMP();
		pumpstate actual_state;
		void run_machine(bool _switch, bool power, bool fail);
		void setstate(pumpstate new_state);
	};

	class PIPELINE
	{
	private:
		bool state_finalized = false;
	public:
		PIPELINE();
		pipelinestate actual_state;
		void run_machine(pumpstate ownpumps[], pumpstate foreignpumps[], valvestate xfeed, valvestate shutoff);
		void setstate(pipelinestate new_state);
	};

}