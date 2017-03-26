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

Reimu::UniversalType::UniversalType(void *v) {
	Type = STRING;
	StringStore += (char *)v;
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
