//
// Created by root on 17-3-24.
//

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

