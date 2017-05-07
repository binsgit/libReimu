//
// Created by root on 17-5-7.
//

#ifndef LIBREIMU_STRINGOPS_HPP
#define LIBREIMU_STRINGOPS_HPP

#include "../libReimu.hpp"

namespace Reimu {
    class StringOps {
    public:
	static std::vector<int64_t> ExtractNumbers(std::string s);
	static std::vector<int64_t> ExtractNumbers(const char *s);
    };
}
#endif //LIBREIMU_STRINGOPS_HPP
