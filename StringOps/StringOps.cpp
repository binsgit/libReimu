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

std::vector<uint8_t>
Reimu::StringOps::EncodingConvert(const void *input, size_t input_len, const char *enc_from, const char *enc_to) {
	std::vector<uint8_t> ret;

	size_t out_len = 4*(input_len+4);
	ret.resize(out_len);
	char *outbuf = (char *)ret.data();

	iconv_t ictx = iconv_open(enc_to, enc_from);

	size_t processed_len;


	while ((processed_len = iconv(ictx, (char **)&input, &input_len, &outbuf, &out_len)) != -1) {
//		std::cerr << "Reimu::StringOps::EncodingConvert: Processed " << processed_len << "chars\n";
		if (input_len == 0)
			break;
	}

	ret.resize(ret.size()-out_len);

	iconv_close(ictx);

	return ret;
}
