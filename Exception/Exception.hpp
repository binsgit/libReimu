//
// Created by root on 17-3-24.
//

#ifndef LIBREIMU_EXCEPTION_HPP
#define LIBREIMU_EXCEPTION_HPP

#include "../libReimu.hpp"

#include <sqlite3.h>
#include <jansson.h>

namespace Reimu {
    class Exception {
    public:

	enum ExceptionType {
	    POSIX = 0x10, SQLite = 0x20, Jansson = 0x40, Custom = 0x80
	};

	ExceptionType Type;

	std::string Message;


	int ErrNo = 0;

	std::string ToString();
	std::string what();

	Exception(int errno_OqO=0);
	Exception(int errno_Sq3, sqlite3 *sq3handle);
	Exception(json_error_t *_json_error);
	Exception(int custom_errno, std::string custom_errmsg);
    };
}

#endif //LIBREIMU_EXCEPTION_HPP
