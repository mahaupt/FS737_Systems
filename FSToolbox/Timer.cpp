#include "Timer.h"

namespace fstoolbox
{
	Timer::Timer(double _timeout, TimeoutStaticCallback func)
	{
		timeout = _timeout;
		callback = nullptr;
		static_callback = func;
		foreign_instance = nullptr;
		enabled = false;

		is_static_callback = true;
	}

	Timer::Timer(double _timeout, TimeoutCallback func, void * instance)
	{
		timeout = _timeout;
		callback = func;
		static_callback = nullptr;
		foreign_instance = instance;
		enabled = false;

		is_static_callback = false;
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

			if (is_static_callback) {
				static_callback();
			} else {
				callback(foreign_instance);
			}
			
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