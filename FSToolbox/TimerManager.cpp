#include "TimerManager.h"

namespace fstoolbox
{
	TimerManager * TimerManager::inst = nullptr;


	TimerManager::TimerManager(double time)
		:timerThread(TimerThread, this)
	{
		timer_interval_s = time;
		last_time = 0;

		//make instance public
		inst = this;
	}

	TimerManager::~TimerManager() {
		running = false;
		timerThread.join();
	}


	// Update is called once per frame
	void TimerManager::TimerThread(TimerManager * instance)
	{
		instance->running = true;

		//initial time
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		std::chrono::high_resolution_clock::time_point t2;
		std::chrono::duration<double> duration;

		while (instance->running) {
			

			//timer
			t2 = std::chrono::high_resolution_clock::now();
			duration = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
			double sleeptime = instance->timer_interval_s - duration.count();
			if (sleeptime < 0) sleeptime = 0;
			mySleep(sleeptime * 1000);
			t1 = std::chrono::high_resolution_clock::now();

			//call timer classes
			if (instance != nullptr)
			{
				for (std::list<Timer*>::iterator it = instance->timerRequestList.begin(); it != instance->timerRequestList.end(); it++)
				{
					(*it)->PassTime(duration.count() + sleeptime);
				}

				//std::cout << (duration.count() + sleeptime) << std::endl;

				for (std::list<TimedCallback>::iterator it = instance->timedCallbackList.begin(); it != instance->timedCallbackList.end(); it++)
				{
					(*it)(duration.count() + sleeptime);
				}
			}
		}
	}


	void TimerManager::addTimer(Timer &timer)
	{
		if (inst != nullptr)
		{
			inst->timerRequestList.push_back(&timer);
		}
	}

	void TimerManager::addTimedCallback(TimedCallback callback) {
		if (inst != nullptr)
		{
			inst->timedCallbackList.push_back(callback);
		}
	}
    
    void TimerManager::mySleep (double ms) {
        #if defined(_WIN32) || defined(_WIN64)
        _sleep(ms);
        #endif
        #if defined(__APPLE__) || defined(__linux__)
        usleep(ms*1000);
        #endif
    }
}
