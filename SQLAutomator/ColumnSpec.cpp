//
// Created by root on 17-3-24.
//

#include "SQLAutomator.hpp"

Reimu::SQLAutomator::ColumnSpec::ColumnSpec(std::string Name, Reimu::SQLAutomator::ColumnSpec::_DataType dataType,
					    size_t Length) {

}

std::string Reimu::SQLAutomator::ColumnSpec::ToString() {
	if (!Cache_S.size())
		Cache_S = Name + " " + ToString(DataType);

	return Cache_S;
}

std::string Reimu::SQLAutomator::ColumnSpec::ToString(Reimu::SQLAutomator::ColumnSpec::_DataType dt) {
	std::string ret;

	if (dt & UNSIGNED)
		ret += "UNSIGNED ";

	if (dt & INTEGER)
		ret += "INTEGER ";

	if (dt & TEXT)
		ret += "TEXT ";

	if (dt & REAL)
		ret += "REAL ";

	if (dt & BLOB)
		ret += "BLOB ";

	if (dt & NOT_NULL)
		ret += "NOT NULL ";

	if (dt & UNIQUE)
		ret += "UNIQUE ";

	if (dt & PRIMARY_KEY)
		ret += "PRIMARY KEY ";

	if (dt & AUTOINCREMENT)
		ret += "AUTOINCREMENT ";

	ret.pop_back();

	return ret;
}

bool const Reimu::SQLAutomator::ColumnSpec::operator==(const Reimu::SQLAutomator::ColumnSpec &o) const {
	return Name == o.Name;
}

bool const Reimu::SQLAutomator::ColumnSpec::operator<(const Reimu::SQLAutomator::ColumnSpec &o) const {
	return Name < o.Name;
}

