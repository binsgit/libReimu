//
// Created by root on 17-4-4.
//

#ifndef LIBREIMU_TASKER_HPP
#define LIBREIMU_TASKER_HPP

#include "../libReimu.hpp"
#include "../Exception/Exception.hpp"

namespace Reimu {
    class Tasker {
    public:

	enum TaskerStatus {
	    Stopped = 0x10, Running = 0x20
	};

	enum TaskerType {
	    FixedTime = 0x10, CountDown = 0x20
	};

	struct tm CurrTime;
	struct tm NextTime;

	int Minute, Hour, Day, Month, Week;

	time_t Interval;

	TaskerStatus Status = Stopped;
	TaskerType Type;

	void (*Func)(void *);
	void *Arg;

	bool WeekOffsetCalibrated = 0;
	bool ShouldRun = 1;
	bool RunImmediately = 1;

	pthread_t ThreadID;
	pthread_attr_t ThreadAttr;

	Tasker();
	Tasker(int m, int h, int dom, int mon, int dow, void (*func)(void *), void *arg, bool run_immediately=1);
	Tasker(time_t interval, void (*func)(void *), void *arg, bool run_immediately=1);

	static void *CountDownThread(void *ptr);
	static void *FixedTimeThread(void *ptr);

	void Start();
	void Stop();
	void Stop_Gracefully();

    private:
	void SetCommonArgs(void (*func)(void *), void *arg, bool run_immediately);
	static void CalcCountdown(Tasker *t);
    };
}
#endif //LIBREIMU_TASKER_HPP
