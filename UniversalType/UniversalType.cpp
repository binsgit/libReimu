//
// Created by root on 17-3-26.
//

#include "UniversalType.hpp"

Reimu::UniversalType::UniversalType(int8_t v) {
	Type = INT8;
	NumericStore.int8 = v;
}

Reimu::UniversalType::UniversalType(uint8_t v) {
	Type = UINT8;
	NumericStore.uint8 = v;
}

Reimu::UniversalType::UniversalType(int16_t v) {
	Type = INT16;
	NumericStore.int16 = v;
}

Reimu::UniversalType::UniversalType(uint16_t v) {
	Type = UINT16;
	NumericStore.uint16 = v;
}

Reimu::UniversalType::UniversalType(int32_t v) {
	Type = INT32;
	NumericStore.int32 = v;
}

Reimu::UniversalType::UniversalType(uint32_t v) {
	Type = UINT32;
	NumericStore.uint32 = v;
}

Reimu::UniversalType::UniversalType(int64_t v) {
	Type = INT64;
	NumericStore.int64 = v;
}

Reimu::UniversalType::UniversalType(uint64_t v) {
	Type = UINT64;
	NumericStore.uint64 = v;
}

Reimu::UniversalType::UniversalType(float v) {
	Type = FLOAT;
	NumericStore.floatOqO = v;
}

Reimu::UniversalType::UniversalType(double v) {
	Type = DOUBLE;
	NumericStore.doubleOqO = v;
}

Reimu::UniversalType::UniversalType(long double v) {
	Type = LONGDOUBLE;
	NumericStore.ldoubleOqO = v;
}

Reimu::UniversalType::UniversalType(std::string v) {
	Type = STRING;
	StringStore = v;
}

Reimu::UniversalType::UniversalType(std::vector<uint8_t> v) {
	Type = BLOB;
	BlobStore = v;
}

Reimu::UniversalType::UniversalType(char *v) {
	Type = STRING;
	StringStore += v;
}


Reimu::UniversalType::UniversalType(void *v) {
	NumericStore.ptr_void = v;
	Type = PTR_VOID;
}

Reimu::UniversalType::UniversalType(void *v, size_t l) {
	Type = BLOB;
	BlobStore.insert(BlobStore.begin(), (uint8_t *)v, (uint8_t *)v+l);
}

void Reimu::UniversalType::Value(void *out) {

	switch (Type) {
		case INT8:
			*(int8_t *)out = NumericStore.int8;
			break;
		case UINT8:
			*(uint8_t *)out = NumericStore.uint8;
			break;
		case INT16:
			*(int16_t *)out = NumericStore.int16;
			break;
		case UINT16:
			*(uint16_t *)out = NumericStore.uint16;
			break;
		case INT32:
			*(int32_t *)out = NumericStore.int32;
			break;
		case UINT32:
			*(uint32_t *)out = NumericStore.uint32;
			break;
		case INT64:
			*(int64_t *)out = NumericStore.int64;
			break;
		case UINT64:
			*(uint64_t *)out = NumericStore.uint64;
			break;
		case FLOAT:
			*(float *)out = NumericStore.floatOqO;
			break;
		case DOUBLE:
			*(double *)out = NumericStore.doubleOqO;
			break;
		case LONGDOUBLE:
			*(long double *)out = NumericStore.ldoubleOqO;
			break;
		case STRING:
			((std::string *)out)->append(StringStore);
			break;
		case BLOB:
			std::vector<uint8_t> *obuf = (std::vector<uint8_t> *)out;
			obuf->insert(obuf->begin(), BlobStore.begin(), BlobStore.end());
			break;
	}
}

Reimu::UniversalType::~UniversalType() {

}

int64_t Reimu::UniversalType::Int64() {
	if (Type > 10)
		throw Reimu::Exception(EINVAL);

	int64_t ret;

	switch (Type) {
		case INT8:
			ret = NumericStore.int8;
			break;
		case UINT8:
			ret = NumericStore.uint8;
			break;
		case INT16:
			ret = NumericStore.int16;
			break;
		case UINT16:
			ret = NumericStore.uint16;
			break;
		case INT32:
			ret = NumericStore.int32;
			break;
		case UINT32:
			ret = NumericStore.uint32;
			break;
		case INT64:
			ret = NumericStore.int64;
			break;
		case UINT64:
			ret = NumericStore.uint64;
			break;
	}

	return ret;
}

double Reimu::UniversalType::Double() {
	if (!(Type > 10 && Type < 20))
		throw Reimu::Exception(EINVAL);

	switch (Type) {
		case FLOAT:
			return (double)NumericStore.floatOqO;
		case DOUBLE:
			return (double)NumericStore.doubleOqO;
		case LONGDOUBLE:
			return (double)NumericStore.ldoubleOqO;
	}
}


std::string Reimu::UniversalType::String() {
	if (Type != STRING)
		throw Reimu::Exception(EINVAL);

	return StringStore;
}

std::vector<uint8_t> Reimu::UniversalType::Blob() {
	if (Type != BLOB)
		throw Reimu::Exception(EINVAL);

	return BlobStore;
}

Reimu::UniversalType::operator int8_t() {
	switch (Type) {
		case INT8:
			return (int8_t)NumericStore.int8;
		case UINT8:
			return (int8_t)NumericStore.uint8;
		case INT16:
			return (int8_t)NumericStore.int16;
		case UINT16:
			return (int8_t)NumericStore.uint16;
		case INT32:
			return (int8_t)NumericStore.int32;
		case UINT32:
			return (int8_t)NumericStore.uint32;
		case INT64:
			return (int8_t)NumericStore.int64;
		case UINT64:
			return (int8_t)NumericStore.uint64;
		case STRING:
			return (int8_t)strtol(StringStore.c_str(), NULL, 10);
		default:
			throw Reimu::Exception(EINVAL);
	}
}

Reimu::UniversalType::operator uint8_t() {
	switch (Type) {
		case INT8:
			return (uint8_t)NumericStore.int8;
		case UINT8:
			return (uint8_t)NumericStore.uint8;
		case INT16:
			return (uint8_t)NumericStore.int16;
		case UINT16:
			return (uint8_t)NumericStore.uint16;
		case INT32:
			return (uint8_t)NumericStore.int32;
		case UINT32:
			return (uint8_t)NumericStore.uint32;
		case INT64:
			return (uint8_t)NumericStore.int64;
		case UINT64:
			return (uint8_t)NumericStore.uint64;
		case STRING:
			return (uint8_t)strtoul(StringStore.c_str(), NULL, 10);
		default:
			throw Reimu::Exception(EINVAL);
	}
}

Reimu::UniversalType::operator int16_t() {
	switch (Type) {
		case INT8:
			return (int16_t)NumericStore.int8;
		case UINT8:
			return (int16_t)NumericStore.uint8;
		case INT16:
			return (int16_t)NumericStore.int16;
		case UINT16:
			return (int16_t)NumericStore.uint16;
		case INT32:
			return (int16_t)NumericStore.int32;
		case UINT32:
			return (int16_t)NumericStore.uint32;
		case INT64:
			return (int16_t)NumericStore.int64;
		case UINT64:
			return (int16_t)NumericStore.uint64;
		case STRING:
			return (int16_t)strtol(StringStore.c_str(), NULL, 10);
		default:
			throw Reimu::Exception(EINVAL);
	}
}

Reimu::UniversalType::operator uint16_t() {
	switch (Type) {
		case INT8:
			return (uint16_t)NumericStore.int8;
		case UINT8:
			return (uint16_t)NumericStore.uint8;
		case INT16:
			return (uint16_t)NumericStore.int16;
		case UINT16:
			return (uint16_t)NumericStore.uint16;
		case INT32:
			return (uint16_t)NumericStore.int32;
		case UINT32:
			return (uint16_t)NumericStore.uint32;
		case INT64:
			return (uint16_t)NumericStore.int64;
		case UINT64:
			return (uint16_t)NumericStore.uint64;
		case STRING:
			return (uint16_t)strtoul(StringStore.c_str(), NULL, 10);
		default:
			throw Reimu::Exception(EINVAL);
	}
}

Reimu::UniversalType::operator int32_t() {
	switch (Type) {
		case INT8:
			return (int32_t)NumericStore.int8;
		case UINT8:
			return (int32_t)NumericStore.uint8;
		case INT16:
			return (int32_t)NumericStore.int16;
		case UINT16:
			return (int32_t)NumericStore.uint16;
		case INT32:
			return (int32_t)NumericStore.int32;
		case UINT32:
			return (int32_t)NumericStore.uint32;
		case INT64:
			return (int32_t)NumericStore.int64;
		case UINT64:
			return (int32_t)NumericStore.uint64;
		case STRING:
			return (int32_t)strtol(StringStore.c_str(), NULL, 10);
		default:
			throw Reimu::Exception(EINVAL);
	}
}

Reimu::UniversalType::operator uint32_t() {
	switch (Type) {
		case INT8:
			return (uint32_t)NumericStore.int8;
		case UINT8:
			return (uint32_t)NumericStore.uint8;
		case INT16:
			return (uint32_t)NumericStore.int16;
		case UINT16:
			return (uint32_t)NumericStore.uint16;
		case INT32:
			return (uint32_t)NumericStore.int32;
		case UINT32:
			return (uint32_t)NumericStore.uint32;
		case INT64:
			return (uint32_t)NumericStore.int64;
		case UINT64:
			return (uint32_t)NumericStore.uint64;
		case STRING:
			return (uint32_t)strtoul(StringStore.c_str(), NULL, 10);
		default:
			throw Reimu::Exception(EINVAL);
	}
}

Reimu::UniversalType::operator int64_t() {
	switch (Type) {
		case INT8:
			return (int64_t)NumericStore.int8;
		case UINT8:
			return (int64_t)NumericStore.uint8;
		case INT16:
			return (int64_t)NumericStore.int16;
		case UINT16:
			return (int64_t)NumericStore.uint16;
		case INT32:
			return (int64_t)NumericStore.int32;
		case UINT32:
			return (int64_t)NumericStore.uint32;
		case INT64:
			return (int64_t)NumericStore.int64;
		case UINT64:
			return (int64_t)NumericStore.uint64;
		case STRING:
			return (int64_t)strtoll(StringStore.c_str(), NULL, 10);
		default:
			throw Reimu::Exception(EINVAL);
	}
}

Reimu::UniversalType::operator uint64_t() {
	switch (Type) {
		case INT8:
			return (uint64_t)NumericStore.int8;
		case UINT8:
			return (uint64_t)NumericStore.uint8;
		case INT16:
			return (uint64_t)NumericStore.int16;
		case UINT16:
			return (uint64_t)NumericStore.uint16;
		case INT32:
			return (uint64_t)NumericStore.int32;
		case UINT32:
			return (uint64_t)NumericStore.uint32;
		case INT64:
			return (uint64_t)NumericStore.int64;
		case UINT64:
			return (uint64_t)NumericStore.uint64;
		case STRING:
			return (uint64_t)strtoull(StringStore.c_str(), NULL, 10);
		default:
			throw Reimu::Exception(EINVAL);
	}
}

Reimu::UniversalType::operator std::string() {
	switch (Type) {
		case INT8:
			return std::to_string(NumericStore.int8);
		case UINT8:
			return std::to_string(NumericStore.uint8);
		case INT16:
			return std::to_string(NumericStore.int16);
		case UINT16:
			return std::to_string(NumericStore.uint16);
		case INT32:
			return std::to_string(NumericStore.int32);
		case UINT32:
			return std::to_string(NumericStore.uint32);
		case INT64:
			return std::to_string(NumericStore.int64);
		case UINT64:
			return std::to_string(NumericStore.uint64);
		case STRING:
			return StringStore;
		default:
			throw Reimu::Exception(EINVAL);
	}
}

Reimu::UniversalType::operator std::vector<uint8_t>() {
	if (Type != BLOB)
		throw Reimu::Exception(EINVAL);
	return BlobStore;
}

Reimu::UniversalType::operator const char *() {
	if (Type == BLOB)
		return (const char *)&BlobStore[0];
	else if (Type == STRING)
		return StringStore.c_str();
}

Reimu::UniversalType::operator void *() {
	if (Type == PTR_VOID)
		return NumericStore.ptr_void;

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
