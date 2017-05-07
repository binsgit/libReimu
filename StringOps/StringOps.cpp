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

std::vector<std::string> Reimu::StringOps::Explode(const char *haystack, const char *needle) {
	return Explode(haystack, strlen(haystack), needle, strlen(needle));
}

std::vector<std::string>
Reimu::StringOps::Explode(const char *haystack, size_t haystack_len, const char *needle, size_t needle_len) {
	std::vector<std::string> ret;
	std::string strbuf;

	const char *stbuf = haystack;
	const char *edbuf;

	while ((edbuf = (const char *)memmem(stbuf, haystack_len-(stbuf-haystack), needle, needle_len))) {
		strbuf.insert(strbuf.begin(), stbuf, edbuf);
		ret.push_back(strbuf);
		strbuf.clear();
		stbuf = edbuf + needle_len;
	}

	strbuf.insert(strbuf.begin(), stbuf, haystack+haystack_len);
	ret.push_back(strbuf);

	return ret;
}
