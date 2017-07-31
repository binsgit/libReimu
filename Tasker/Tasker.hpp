/*
    This file is part of libReimu.
    Copyright (C) 2016-2017  CloudyReimu <cloudyreimu@gmail.com>

    libReimu is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libReimu is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libReimu.  If not, see <http://www.gnu.org/licenses/>.

    Tasker - A cron-like task scheduler.
*/

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

	std::string Name;

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

	bool const operator== (const Reimu::Tasker &o) const;
	bool const operator< (const Reimu::Tasker &o) const;

	Tasker();
	Tasker(std::string name, int m, int h, int dom, int mon, int dow, void (*func)(void *), void *arg, bool run_immediately=1);
	Tasker(std::string name, time_t interval, void (*func)(void *), void *arg, bool run_immediately=1);

	static void *CountDownThread(void *ptr);
	static void *FixedTimeThread(void *ptr);

	void Start();
	void Stop();
	void Stop_Gracefully();

    private:
	void SetCommonArgs(void (*func)(void *), void *arg, bool run_immediately);
	static void CalcCountdown(Tasker *t);

	pthread_mutex_t CancelLock = PTHREAD_MUTEX_INITIALIZER;
    };
}
#endif //LIBREIMU_TASKER_HPP
