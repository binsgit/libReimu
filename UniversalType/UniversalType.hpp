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
    public:
	enum Types {
	    INT8 = 1, UINT8 = 2, INT16 = 3, UINT16 = 4, INT32 = 5, UINT32 = 6, INT64 = 7, UINT64 = 8, BOOL = 9,
	    FLOAT = 11, DOUBLE = 12, LONGDOUBLE = 13,
	    STRING = 21, BLOB = 31,
	    PTR_VOID = 40
	};

	long double NumericStore;

	std::vector<uint8_t> BlobStore;
	std::string StringStore;

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

	explicit UniversalType(void *v);

	~UniversalType();

	operator int64_t();

	explicit operator double();
	explicit operator float();
	explicit operator long double();

//	operator Reimu::UniversalType();

	explicit operator std::vector<uint8_t>();
	explicit operator std::vector<uint8_t> *();
	explicit operator std::pair<void *, size_t>();
	explicit operator std::string();
	explicit operator std::string *();
	explicit operator const char *();
	operator void *();

    };
}
#endif //LIBREIMU_UNIVERSALTYPE_HPP
