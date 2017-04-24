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

#include "Tasker.hpp"

Reimu::Tasker::Tasker() {

}

Reimu::Tasker::Tasker(std::string name, int m, int h, int dom, int mon, int dow, void (*func)(void *), void *arg, bool run_immediately) {
	Name = name;

	Minute = m;
	Hour = h;
	Day = dom;
	Month = mon;
	Week = dow;

	Type = FixedTime;
	SetCommonArgs(func, arg, run_immediately);
}

Reimu::Tasker::Tasker(std::string name, time_t interval, void (*func)(void *), void *arg, bool run_immediately) {
	Name = name;

	Interval = interval;

	Type = CountDown;
	SetCommonArgs(func, arg, run_immediately);
}

void Reimu::Tasker::SetCommonArgs(void (*func)(void *), void *arg, bool run_immediately) {
	RunImmediately = run_immediately;

	Func = func;
	Arg = arg;

	memset(&NextTime, 0, sizeof(tm));
	memset(&CurrTime, 0, sizeof(tm));
	pthread_attr_init(&ThreadAttr);
	pthread_attr_setdetachstate(&ThreadAttr, PTHREAD_CREATE_DETACHED);
}


void Reimu::Tasker::Start() {
	switch (Type) {
		case FixedTime:
			pthread_create(&ThreadID, &ThreadAttr, &FixedTimeThread, this);
			break;
		case CountDown:
			pthread_create(&ThreadID, &ThreadAttr, &CountDownThread, this);
			break;
	}
}

void Reimu::Tasker::Stop() {
	pthread_cancel(ThreadID);
}

void Reimu::Tasker::Stop_Gracefully() {
	ShouldRun = 0;
}

void *Reimu::Tasker::CountDownThread(void *ptr) {
	Tasker *thisTask = (Tasker *)ptr;

	if (thisTask->RunImmediately)
		thisTask->Func(thisTask->Arg);

	while (thisTask->ShouldRun) {
		sleep((uint)thisTask->Interval);
		thisTask->Func(thisTask->Arg);
	}

	pthread_exit(NULL);
}

void *Reimu::Tasker::FixedTimeThread(void *ptr) {
	Tasker *thisTask = (Tasker *)ptr;

	if (thisTask->RunImmediately)
		thisTask->Func(thisTask->Arg);

	while (thisTask->ShouldRun) {
		CalcCountdown(thisTask);
		sleep((uint)thisTask->Interval);
		thisTask->Func(thisTask->Arg);
	}

	pthread_exit(NULL);
}

void Reimu::Tasker::CalcCountdown(Tasker *t) {
	time_t timenooow = time(NULL);


	localtime_r(&timenooow, &t->CurrTime);
	memcpy(&t->NextTime, &t->CurrTime, sizeof(tm));

	if (t->Minute == -1) {
		if (t->CurrTime.tm_min != 59) {
			t->NextTime.tm_min = t->CurrTime.tm_min + 1;
			goto gentime;
		} else {
			t->NextTime.tm_min = 0;
		}
	} else {
		t->NextTime.tm_min = t->Minute;
	}

	if (t->Hour == -1) {
		if (t->CurrTime.tm_hour != 23) {
			t->NextTime.tm_hour = t->CurrTime.tm_hour + 1;
			goto gentime;
		} else {
			t->NextTime.tm_hour = 0;
		}
	} else {
		t->NextTime.tm_hour = t->Hour;
	}

	if (t->Day == -1) {
		if (t->NextTime.tm_mday) {
			if (t->WeekOffsetCalibrated)
				t->NextTime.tm_mday += 7;
			else
				t->NextTime.tm_mday = t->CurrTime.tm_mday + 1;
		} else {
			t->NextTime.tm_mday = t->CurrTime.tm_mday;
		}
	} else
		t->NextTime.tm_mday = t->Day;

//	if (t->Month == -1)
//		if (t->NextTime.tm_mday == 30) {
//		t->NextTime.tm_mon = t->CurrTime.tm_mon+1;
//	else
	// TODO
	t->NextTime.tm_mon = t->CurrTime.tm_mon+1;

	t->NextTime.tm_year = t->CurrTime.tm_year;

	gentime:
	t->Interval = mktime(&t->NextTime)-timenooow;

	if (t->Day == -1 && t->Week != -1) {
		while (t->NextTime.tm_wday != t->Week) {
			t->NextTime.tm_mday++;
			t->Interval = mktime(&t->NextTime);
		}
		t->WeekOffsetCalibrated = 1;
	}
	fprintf(stderr, "[Tasker @ %p] Reimu::Tasker::CalcCountdown: %d-%d-%d %d:%d\n", t,
		t->NextTime.tm_year+1900, t->NextTime.tm_mon, t->NextTime.tm_mday, t->NextTime.tm_hour, t->NextTime.tm_min);
	fprintf(stderr, "[Tasker @ %p] Reimu::Tasker::CalcCountdown: %lu seconds\n", t, t->Interval);
}

bool const Reimu::Tasker::operator==(const Reimu::Tasker &o) const {
	return Name == o.Name;
}

bool const Reimu::Tasker::operator<(const Reimu::Tasker &o) const {
	return Name < o.Name;
}







