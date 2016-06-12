#pragma once
#include <list>
#include <thread>
#include <chrono>

#include "Timer.h"

/*
*
* This TimerManager class provides a clock for the normal timer classes.
* In the method Update, it updates all timer in the Timer class instances.
* It has to be created once, to enable Timer usage.
*
*/

namespace fstoolbox {
	typedef void(*TimedCallback)(double);

	class TimerManager
	{
	private:
		//Sample time in ms
		double timer_interval_s;
		//save last time to get time difference
		double last_time;
		bool running;

		std::list<Timer> timerRequestList;
		std::list<TimedCallback> timedCallbackList;
		std::thread timerThread;
	public:
		static TimerManager * inst;

		TimerManager(double time);
		~TimerManager();


		// Update is called once per frame
		static void TimerThread(TimerManager * instance);
		static void addTimer(Timer &timer);
		static void addTimedCallback(TimedCallback callback);
	};
}

