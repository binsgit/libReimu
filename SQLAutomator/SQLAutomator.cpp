//
// Created by root on 17-3-24.
//

#include "SQLAutomator.hpp"

std::string Reimu::SQLAutomator::Statement(Reimu::SQLAutomator::StatmentType st) {

	bool InvalidateCaches = 0;

	if (Columns.size() != LastCount_Columns) {
		InvalidateCaches = 1;
	}

	Cache_Statements[st];

	switch (st) {
		case CREATE_TABLE:
			if ()
	}

	return std::string();
}
