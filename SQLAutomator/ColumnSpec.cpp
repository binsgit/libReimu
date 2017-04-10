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

    SQLAutomator - Makes life with SQL databases easier.
*/

#include "SQLAutomator.hpp"

Reimu::SQLAutomator::ColumnSpec::ColumnSpec() {

}

Reimu::SQLAutomator::ColumnSpec::ColumnSpec(std::string col_name, int dataType,
					    size_t datalength) {
	Name = col_name;
	DataType = (Reimu::SQLAutomator::ColumnSpec::Type)dataType;
	Length = datalength;
}

std::string Reimu::SQLAutomator::ColumnSpec::ToString() {
	if (!Cache_S.size())
		Cache_S = Name + " " + ToString(DataType, Length);

	return Cache_S;
}

std::string Reimu::SQLAutomator::ColumnSpec::ToString(Reimu::SQLAutomator::ColumnSpec::Type dt, size_t datalength) {
	std::string ret;

	if (dt & UNSIGNED)
		ret += "UNSIGNED ";

	if (dt & INTEGER)
		ret += "INTEGER ";
	else if (dt & TEXT)
		ret += "TEXT ";
	else if (dt & REAL)
		ret += "REAL ";
	else if (dt & BLOB)
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

