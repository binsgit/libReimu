//
// Created by root on 17-4-24.
//

#include "DateTime.hpp"

int Reimu::DateTime::DaysCount(int month, int year) {
	int days;

	if (month == 4 || month == 6 || month == 9 || month == 11)
		days = 30;
	else if (month == 2) {
		bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
		if (isLeapYear)
			days = 29;
		else
			days = 28;
	} else
		days = 31;

	return days;
}
