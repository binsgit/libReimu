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
	static std::vector<std::string> Explode(const char *haystack, const char *needle) __attribute__ ((warning("Be care of strings that contain zeros (e.g. UTF16) !")));
	static std::vector<std::string> Explode(const char *haystack, size_t haystack_len, const char *needle, size_t needle_len);

	static std::vector<uint8_t> EncodingConvert(const void *input, size_t input_len, const char *enc_from, const char *enc_to);
    };
}
#endif //LIBREIMU_STRINGOPS_HPP
