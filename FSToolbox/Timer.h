#pragma once
namespace fstoolbox
{
	typedef void(*TimeoutCallback)(void);

	class Timer
	{
	private:
		TimeoutCallback callback;
		double timeout;
		double current_time;
		bool enabled;

	public:
		Timer(double _timeout, TimeoutCallback func);

		//called when TimerManager samples time
		void PassTime(double _sec);
		void Reset();
		void Start();
		void Stop();
		bool isEnabled();
	};
}
