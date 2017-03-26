//
// Created by root on 17-3-24.
//

#include "Exception.hpp"



const char *Reimu::Exception::ToString() {
	return strerror(ErrNo);
}

Reimu::Exception::Exception(int errno_OqO) {
	ErrNo = errno_OqO;
}
