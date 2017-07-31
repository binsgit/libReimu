//
// Created by root on 17-5-31.
//

#ifndef LIBREIMU_INT128_T_HPP
#define LIBREIMU_INT128_T_HPP

#include "../libReimu.hpp"

namespace Reimu {
    class int128_t {
    public:
	__int128 data;

	bool const operator== (const Reimu::int128_t &o) const {
		return data == o.data;
	}

	bool const operator!= (const Reimu::int128_t &o) const {
		return data != o.data;
	}

	bool const operator< (const Reimu::int128_t &o) const {
		return data < o.data;
	}

	bool const operator> (const Reimu::int128_t &o) const {
		return data > o.data;
	}

	std::size_t operator()(const Reimu::int128_t& k) const {
		return k.data / INT64_MAX;
	}


	int128_t() {}
	int128_t(int64_t v) { data = v; }

    };
}
#endif //LIBREIMU_INT128_T_HPP
