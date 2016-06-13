#pragma once
namespace fstoolbox
{
	typedef void(*TimeoutStaticCallback)(void);
	typedef void(*TimeoutCallback)(void*);

	class Timer
	{
	private:
		TimeoutCallback callback;
		TimeoutStaticCallback static_callback;
		void * foreign_instance;

		double is_static_callback;
		double timeout;
		double current_time;
		bool enabled;

	public:
		Timer(double _timeout, TimeoutStaticCallback func);
		Timer(double _timeout, TimeoutCallback func, void* instance);

		//called when TimerManager samples time
		void PassTime(double _sec);
		void Reset();
		void Start();
		void Stop();
		bool isEnabled();
	};
}
