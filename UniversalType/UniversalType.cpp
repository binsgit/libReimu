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

#include "UniversalType.hpp"

Reimu::UniversalType::UniversalType(bool v) {
	Type = BOOL;

	if (v)
		*((uint64_t *)&NumericStore) = 1;
	else
		*((uint64_t *)&NumericStore) = 0;
}

Reimu::UniversalType::UniversalType(uint8_t v) {
	Type = UINT8;
	*((uint8_t *)&NumericStore) = v;
}

Reimu::UniversalType::UniversalType(int8_t v) {
	Type = INT8;
	*((int8_t *)&NumericStore) = v;
}

Reimu::UniversalType::UniversalType(uint16_t v) {
	Type = UINT16;
	*((uint16_t *)&NumericStore) = v;
}

Reimu::UniversalType::UniversalType(int16_t v) {
	Type = INT16;
	*((int16_t *)&NumericStore) = v;
}

Reimu::UniversalType::UniversalType(uint32_t v) {
	Type = UINT32;
	*((uint32_t *)&NumericStore) = v;
}

Reimu::UniversalType::UniversalType(int32_t v) {
	Type = INT32;
	*((int32_t *)&NumericStore) = v;
}

Reimu::UniversalType::UniversalType(uint64_t v) {
	Type = UINT64;
	*((uint64_t *)&NumericStore) = v;
}

Reimu::UniversalType::UniversalType(int64_t v) {
	Type = INT64;
	*((int64_t *)&NumericStore) = v;
}

Reimu::UniversalType::UniversalType(float v) {
	Type = FLOAT;
	NumericStore = v;
}

Reimu::UniversalType::UniversalType(double v) {
	Type = DOUBLE;
	NumericStore = v;
}

Reimu::UniversalType::UniversalType(long double v) {
	Type = LONGDOUBLE;
	NumericStore = v;
}

Reimu::UniversalType::UniversalType(std::string v) {
	Type = STRING;
	StringStore.operator=(v);
}

Reimu::UniversalType::UniversalType(std::vector<uint8_t> v) {
	Type = BLOB;
	BlobStore.operator=(v);
}

Reimu::UniversalType::UniversalType(char *v, bool deepcopy) {
	Type = STRING;

	if (deepcopy) {
		StringStore.operator+=(v);
	} else {
		*((int64_t *)&NumericStore) = (int64_t)v;
		ShadowSize = strlen(v);
	}
}


Reimu::UniversalType::UniversalType(void *v) {
	*((int64_t *)&NumericStore) = (int64_t)v;
	Type = PTR_VOID;
}

Reimu::UniversalType::UniversalType(void *v, size_t l, bool deepcopy) {
	Type = BLOB;

	fprintf(stderr, "[%s @ %p] v=%p, l=%zu\n", __PRETTY_FUNCTION__, this, v, l);
	if (!deepcopy) {
		ShadowSize = l;
		*((int64_t *)&NumericStore) = (int64_t)v;
	} else {
		BlobStore.insert(BlobStore.begin(), (uint8_t *) v, (uint8_t *) v + l);
	}
}


Reimu::UniversalType::~UniversalType() {


}

Reimu::UniversalType::operator uint64_t() {
	if (Type < 10) {
		return *((uint64_t *)&NumericStore);
	} else {
		if (Type < 20) {
			return (uint64_t) NumericStore;
		} else if (Type == STRING) {
			if (StringStore.size())
				if (StringStore.operator[](0) == '-')
					return strtoll(StringStore.c_str(), NULL, 10);
				else
					return (int64_t)strtoull(StringStore.c_str(), NULL, 10);
		} else {
			throw Reimu::Exception(EINVAL);
		}
	}
}

Reimu::UniversalType::operator int64_t() {
	if (Type < 10) {
		return *((int64_t *)&NumericStore);
	} else {
		if (Type < 20) {
			return (int64_t) NumericStore;
		} else if (Type == STRING) {
			if (StringStore.size())
				if (StringStore.operator[](0) == '-')
					return strtoll(StringStore.c_str(), NULL, 10);
				else
					return (int64_t)strtoull(StringStore.c_str(), NULL, 10);
		} else {
			throw Reimu::Exception(EINVAL);
		}
	}
}


Reimu::UniversalType::operator double() {
	if (Type < 10) {
		return *((int64_t *)&NumericStore);
	} else {
		if (Type < 20) {
			return (double)NumericStore;
		} else if (Type == STRING) {
			if (StringStore.size())
				return strtod(StringStore.c_str(), NULL);
			else
				throw Reimu::Exception(EINVAL);
		} else {
			throw Reimu::Exception(EINVAL);
		}
	}
}

Reimu::UniversalType::operator float() {
	if (Type < 10) {
		return *((int64_t *)&NumericStore);
	} else {
		if (Type < 20) {
			return (float)NumericStore;
		} else if (Type == STRING) {
			if (StringStore.size())
			return strtof(StringStore.c_str(), NULL);
			else
				throw Reimu::Exception(EINVAL);
		} else {
			throw Reimu::Exception(EINVAL);
		}
	}
}

Reimu::UniversalType::operator long double() {
	if (Type < 10) {
		return *((int64_t *)&NumericStore);
	} else {
		if (Type < 20) {
			return NumericStore;
		} else if (Type == STRING) {
			if (StringStore.size())
			return strtold(StringStore.c_str(), NULL);
			else
				throw Reimu::Exception(EINVAL);
		} else {
			throw Reimu::Exception(EINVAL);
		}
	}
}

Reimu::UniversalType::operator std::string() {

	if (Type < 10) {
		if (Type % 2)
			return std::to_string((int64_t)*((int64_t *)&NumericStore));
		else
			return std::to_string((uint64_t)*((int64_t *)&NumericStore));
	} else {
		if (Type < 20) {
			return std::to_string(NumericStore);
		} else if (Type == STRING) {
			if (ShadowSize)
				return std::string((char *)*(int64_t *)&NumericStore);
			else
				return StringStore;
		} else if (Type == PTR_VOID) {
			char sbuf[32] = {0};
			snprintf(sbuf, 31, "%p", (void *)*(int64_t *)&NumericStore);
			return std::string(sbuf);
		} else {
			throw Reimu::Exception(EINVAL);
		}
	}

}

Reimu::UniversalType::operator std::vector<uint8_t>() {
	if (Type != BLOB)
		throw Reimu::Exception(EINVAL);

	if (ShadowSize)
		return std::vector<uint8_t>((uint8_t *)*(int64_t *)&NumericStore, (uint8_t *)*(int64_t *)&NumericStore+ShadowSize);

	return BlobStore;
}

Reimu::UniversalType::operator const char *() {
	if (Type == BLOB)
		return (const char *)&BlobStore[0];
	else if (Type == STRING)
		if (ShadowSize)
			return (const char *)*(int64_t *)&NumericStore;
		else
			return StringStore.c_str();
}

Reimu::UniversalType::operator void *() {
	if (ShadowSize && Type == PTR_VOID)
		return (void *)*((int64_t *)&NumericStore);

	if (Type == BLOB)
			return &BlobStore[0];


	if (Type == STRING)
		return (void *)StringStore.c_str();
}

Reimu::UniversalType::operator std::vector<uint8_t> *() {
	if (Type != BLOB)
		throw Reimu::Exception(EINVAL);

	return &BlobStore;
}

Reimu::UniversalType::operator std::string *() {
	if (Type != STRING)
		throw Reimu::Exception(EINVAL);

	return &StringStore;
}

Reimu::UniversalType::operator std::pair<void *, size_t>() {
	if (Type != BLOB)
		throw Reimu::Exception(EINVAL);

	if (ShadowSize)
		return std::pair<void *, size_t>((void *)*((int64_t *)&NumericStore), ShadowSize);

	return std::pair<void *, size_t>(&(BlobStore[0]), BlobStore.size());
}

size_t Reimu::UniversalType::Size() {
	if (ShadowSize)
		return ShadowSize;

	if (Type == BLOB)
		return BlobStore.size();

	if (Type == STRING)
		return StringStore.size();

	switch (Type) {
		case INT8:
			return sizeof(int8_t);
		case UINT8:
			return sizeof(uint8_t);
		case INT16:
			return sizeof(int16_t);
		case UINT16:
			return sizeof(uint16_t);
		case INT32:
			return sizeof(int32_t);
		case UINT32:
			return sizeof(uint32_t);
		case INT64:
			return sizeof(int64_t);
		case UINT64:
			return sizeof(uint64_t);
		case FLOAT:
			return sizeof(float);
		case DOUBLE:
			return sizeof(double);
		case LONGDOUBLE:
			return sizeof(long double);
		default:
			return 0;
	}

}



//Reimu::UniversalType::operator Reimu::UniversalType() {
//	return this;
//}
