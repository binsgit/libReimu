//
// Created by root on 17-4-4.
//

#include "Tasker.hpp"

Reimu::Tasker::Tasker() {

}

Reimu::Tasker::Tasker(int m, int h, int dom, int mon, int dow, void (*func)(void *), void *arg, bool run_immediately) {
	Minute = m;
	Hour = h;
	Day = dom;
	Month = mon;
	Week = dow;

	SetCommonArgs(func, arg, run_immediately);
}

Reimu::Tasker::Tasker(time_t interval, void (*func)(void *), void *arg, bool run_immediately) {
	Interval = interval;

	SetCommonArgs(func, arg, run_immediately);
}

void Reimu::Tasker::SetCommonArgs(void (*func)(void *), void *arg, bool run_immediately) {
	RunImmediately = run_immediately;

	Func = func;
	Arg = arg;

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

	gmtime_r(&timenooow, &t->CurrTime);

	if (t->Minute == -1)
		t->NextTime.tm_min = t->CurrTime.tm_min+1;
	else
		t->NextTime.tm_min = t->Minute;

	if (t->Hour == -1)
		t->NextTime.tm_hour = t->CurrTime.tm_hour+1;
	else
		t->NextTime.tm_hour = t->Hour;

	if (t->Day == -1) {
		if (t->WeekOffsetCalibrated)
			t->NextTime.tm_mday += 7;
		else
			t->NextTime.tm_mday = t->CurrTime.tm_mday + 1;
	} else
		t->NextTime.tm_mday = t->Day;

	if (t->Month == -1)
		t->NextTime.tm_mon = t->CurrTime.tm_mon+1;
	else
		t->NextTime.tm_mon = t->Month;



	t->Interval = mktime(&t->NextTime);

	if (t->Day == -1 && t->Week != -1) {
		while (t->NextTime.tm_wday != t->Week) {
			t->NextTime.tm_mday++;
			t->Interval = mktime(&t->NextTime);
		}
		t->WeekOffsetCalibrated = 1;
	}
}







