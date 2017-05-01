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

    UniversalType - A versatile value holder that eases your life.
*/

#ifndef LIBREIMU_UNIVERSALTYPE_HPP
#define LIBREIMU_UNIVERSALTYPE_HPP

#include "../libReimu.hpp"
#include "../Exception/Exception.hpp"

namespace Reimu {
    class UniversalType {
    private:
	template<typename T> // Intentionally
	UniversalType(T arg1);

	template<typename T> // Intentionally
	operator T();
    public:
	enum Types {
	    INT8 = 1, UINT8 = 2, INT16 = 3, UINT16 = 4, INT32 = 5, UINT32 = 6, INT64 = 7, UINT64 = 8, BOOL = 9,
	    FLOAT = 11, DOUBLE = 12, LONGDOUBLE = 13,
	    STRING = 21, BLOB = 31,
	    PTR_VOID = 40
	};

	long double NumericStore;

	std::vector<uint8_t> *BlobStore = NULL;
	std::string *StringStore = NULL;

	size_t ShadowSize = 0;

	Types Type;

	UniversalType(bool v);
	UniversalType(int8_t v);
	UniversalType(uint8_t v);
	UniversalType(int16_t v);
	UniversalType(uint16_t v);
	UniversalType(int32_t v);
	UniversalType(uint32_t v);
	UniversalType(int64_t v);
	UniversalType(uint64_t v);

	UniversalType(float v);
	UniversalType(double v);
	UniversalType(long double v);

	UniversalType(std::string v);
	UniversalType(std::vector<uint8_t> v);
	UniversalType(char *v, bool deepcopy=1);
	UniversalType(void *v, size_t l, bool deepcopy=1);

	UniversalType(void *v);

	UniversalType(const UniversalType &other);
//	UniversalType(const UniversalType &&other);
	UniversalType& operator= (UniversalType other);

	~UniversalType();

	size_t Size();

	operator Reimu::UniversalType *(){
		return this;
	}

	operator int64_t();
	operator uint64_t();

	inline operator int8_t() {
		return (int8_t)operator int64_t();
	}

	inline operator uint8_t() {
		return (uint8_t)operator uint64_t();
	}

	inline operator int16_t() {
		return (int16_t)operator int64_t();
	}

	inline operator uint16_t() {
		return (uint16_t)operator uint64_t();
	}

	inline operator int32_t() {
		return (int32_t)operator int64_t();
	}

	inline operator uint32_t() {
		return (uint32_t)operator uint64_t();
	}

//	operator short int() {
//		return int16_t();
//	}
//
//	operator unsigned short int() {
//		return uint16_t();
//	}
//
//	operator long int() {
//		return int64_t();
//	}
//
//	operator unsigned long int() {
//		return uint64_t();
//	}
//
	inline operator long long int() {
		return operator int64_t();
	}

	inline operator unsigned long long int() {
		return operator uint64_t();
	}

	operator double();
	operator float();
	operator long double();

//	operator Reimu::UniversalType();

	operator std::vector<uint8_t>();
	operator std::vector<uint8_t> *();
	operator std::pair<void *, size_t>();
	operator std::string();
	operator std::string *();
	operator const char *();
	operator void *();

    };
}
#endif //LIBREIMU_UNIVERSALTYPE_HPP
