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

#include "Exception.hpp"

std::string Reimu::Exception::ToString() {
	switch (Type) {
		case POSIX:
			return strerror(ErrNo);
		case SQLite:
			return Message;
		case Jansson:
			return Message;
	}

}

std::string Reimu::Exception::what() {
	return ToString();
}


Reimu::Exception::Exception(int errno_OqO) {
	ErrNo = errno_OqO;
	Type = POSIX;
}

Reimu::Exception::Exception(int errno_Sq3, sqlite3 *sq3handle) {
	ErrNo = errno_Sq3;
	Type = SQLite;
	if (sq3handle)
		Message += sqlite3_errmsg(sq3handle);
}

Reimu::Exception::Exception(json_error_t *_json_error) {
	ErrNo = -1;
	Type = Jansson;

	if (_json_error) {
		Message += _json_error->text;
		Message += " (line " + std::to_string(_json_error->line) + " column " +
			   std::to_string(_json_error->column) + ")";
	}
}

Reimu::Exception::Exception(int custom_errno, std::string custom_errmsg) {
	ErrNo = custom_errno;
	Type = Custom;
	Message = custom_errmsg;
}

