//
// Created by root on 17-5-2.
//

#ifndef LIBREIMU_LOGGING_HPP
#define LIBREIMU_LOGGING_HPP

#include "../libReimu.hpp"
#include "../UniversalType/UniversalType.hpp"

namespace Kanna {
    class Logging {
    public:

	enum LogType {
	    Critical = 10, Error = 20, Warning = 30, Notice = 40, Info = 50, Debug = 60
	};

	FILE *LogFile = NULL;

	pthread_mutex_t Lock = PTHREAD_MUTEX_INITIALIZER;

	std::stringstream SSBuf;

	Logging();
	Logging(std::string logfile);

	void Log(int type, const char *format, ...);


	std::stringstream& operator<< (int type);

    };
}

#endif //LIBREIMU_LOGGING_HPP
