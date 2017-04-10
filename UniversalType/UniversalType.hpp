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
    public:
	enum Types {
	    INT8 = 1, UINT8 = 2, INT16 = 3, UINT16 = 4, INT32 = 5, UINT32 = 6, INT64 = 7, UINT64 = 8,
	    FLOAT = 11, DOUBLE = 12, LONGDOUBLE = 13,
	    STRING = 21, BLOB = 31,
	    PTR_VOID = 40
	};

	union OqO {
	    int8_t int8;
	    uint8_t uint8;
	    int16_t int16;
	    uint16_t uint16;
	    int32_t int32;
	    uint32_t uint32;
	    int64_t int64;
	    uint64_t uint64;
	    float floatOqO;
	    double doubleOqO;
	    long double ldoubleOqO;
	    void *ptr_void;
	} NumericStore;

	std::vector<uint8_t> BlobStore;
	std::string StringStore;

	Types Type;

	UniversalType(int8_t v);
	UniversalType(int16_t v);
	UniversalType(int32_t v);
	UniversalType(int64_t v);
	UniversalType(uint8_t v);
	UniversalType(uint16_t v);
	UniversalType(uint32_t v);
	UniversalType(uint64_t v);

	UniversalType(float v);
	UniversalType(double v);
	UniversalType(long double v);

	UniversalType(std::string v);
	UniversalType(std::vector<uint8_t> v);
	UniversalType(char *v);
	UniversalType(void *v, size_t l);

	UniversalType(void *v);

	~UniversalType();

	void Value(void *out);
	int64_t Int64();
	double Double();
	std::string String();
	std::vector<uint8_t> Blob();

	operator int8_t();
	operator uint8_t();
	operator int16_t();
	operator uint16_t();
	operator int32_t();
	operator uint32_t();
	operator int64_t();
	operator uint64_t();

	operator std::vector<uint8_t>();
	operator std::vector<uint8_t> *();
	operator std::string();
	operator std::string *();
	operator const char *();
	operator void *();

    };
}
#endif //LIBREIMU_UNIVERSALTYPE_HPP
