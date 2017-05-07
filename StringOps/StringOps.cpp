//
// Created by root on 17-5-7.
//

#include "StringOps.hpp"

std::vector<int64_t> Reimu::StringOps::ExtractNumbers(std::string s) {
	return ExtractNumbers(s.c_str());
}

std::vector<int64_t> Reimu::StringOps::ExtractNumbers(const char *s) {
	std::vector<int64_t> ret;
	int64_t num;
	char *buf = (char *)s;

	while (*buf) {
		if (*buf >= '0' && *buf <= '9') {
			num = strtoll(buf, &buf, 10);
			ret.push_back(num);
		} else {
			buf++;
		}
	}

	return ret;
}
