#include "Timer.h"

namespace fsitoolbox
{
	Timer::Timer(double _timeout, TimeoutCallback func)
	{
		timeout = _timeout;
		callback = func;
	}


	//called when TimerManager samples time
	void Timer::PassTime(double _sec)
	{
		//if timer is not enabled -- abort
		if (!enabled) return;

		current_time += _sec;

		//Timer is expired, call callback
		if (current_time >= timeout)
		{
			Reset();
			callback();
		}
	}

	void Timer::Reset()
	{
		enabled = false;
		current_time = 0;
	}

	void Timer::Start()
	{
		enabled = true;
	}

	void Timer::Stop()
	{
		enabled = false;
	}

	bool Timer::isEnabled()
	{
		return enabled;
	}
}