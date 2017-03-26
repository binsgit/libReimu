//
// Created by root on 17-3-24.
//

#ifndef LIBREIMU_EXCEPTION_HPP
#define LIBREIMU_EXCEPTION_HPP

#include "../libReimu.hpp"

namespace Reimu {
    class Exception {
    public:
	int ErrNo = 0;

	const char *ToString();

	Exception(int errno_OqO=0);
    };
}

#endif //LIBREIMU_EXCEPTION_HPP
