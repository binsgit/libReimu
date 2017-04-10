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

    Exception - Throw elegantly.

    "Throwing an integer is a bad programming practise." Nonsense!!
*/

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
