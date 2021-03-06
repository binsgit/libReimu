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

#include "../SQLAutomator.hpp"

Reimu::SQLAutomator::SQLite3::SQLite3() {

}

Reimu::SQLAutomator::SQLite3::SQLite3(std::string db_uri, int flags, char *vfs, Reimu::SQLAutomator *parent) {
	int rc = Open(db_uri, flags, vfs);
	if (SQLITE_OK != rc)
		throw Reimu::Exception(rc, SQLite3DB);
	Parent = parent;
}

int Reimu::SQLAutomator::SQLite3::Open(std::string db_uri, int flags, char *vfs) {
	int ret = sqlite3_open_v2(db_uri.c_str(), &SQLite3DB, flags, (const char *)vfs);

#ifdef Reimu_DEBUG
	fprintf(stderr, "[%s @ %p] db_uri=%s, ret=%d, sqlite3 *=%p\n", __PRETTY_FUNCTION__, this, db_uri.c_str(), ret, SQLite3DB);
#endif

	if (ret != 0)
		SQLite3DB = NULL;

	return ret;
}


int Reimu::SQLAutomator::SQLite3::Bind(size_t narg, Reimu::UniversalType thisval) {
#ifndef Reimu_DEBUG
	fprintf(stderr, "[SQLite3::Bind @ %p] Column=%zu Type=%d d64=%ld ptr=%p\n", this, narg, thisval.Type, *(int64_t *)&thisval.NumericStore, (void *)*(uintptr_t *)&thisval.NumericStore);
#endif
	if (thisval.Type < 10) {
		return sqlite3_bind_int64(SQLite3Statement, (int)narg, thisval.operator int64_t());
	} else if (thisval.Type > 10 && thisval.Type < 20) {
		return sqlite3_bind_double(SQLite3Statement, (int)narg, thisval.operator double());
	} else if (thisval.Type == Reimu::UniversalType::Types::STRING) {
		return sqlite3_bind_text(SQLite3Statement, (int)narg, thisval.StringStore->c_str(), (int)thisval.StringStore->size(), SQLITE_TRANSIENT);
	} else if (thisval.Type == Reimu::UniversalType::Types::BLOB) {
		return sqlite3_bind_blob64(SQLite3Statement, (int)narg, thisval.BlobStore->data(), thisval.BlobStore->size(), SQLITE_TRANSIENT);
	} else {
		throw Reimu::Exception(EINVAL);
	}
}

void Reimu::SQLAutomator::SQLite3::Bind() {
	size_t j = 1;

	for (auto &thisval : Values) {
		Bind(j, thisval);
		j++;
	}
}

int Reimu::SQLAutomator::SQLite3::Prepare() {
	if (SQLite3Statement) {
		sqlite3_finalize(SQLite3Statement);
		SQLite3Statement = NULL;
	}
	int ret = sqlite3_prepare(SQLite3DB, Statement.c_str(), (int)Statement.length(), &SQLite3Statement, NULL);
#ifdef Reimu_DEBUG
	fprintf(stderr, "[%s @ %p] stmt_str=%s, ret=%d, s3=%p, s3stmt=%p\n", __PRETTY_FUNCTION__, this, Statement.c_str(),
		ret, SQLite3DB, SQLite3Statement);
#endif


	return ret;
}

int Reimu::SQLAutomator::SQLite3::Prepare(std::string stmt_str) {
	Statement = stmt_str;
	return Prepare();
}

int Reimu::SQLAutomator::SQLite3::Reset() {
	return sqlite3_reset(SQLite3Statement);
}

int Reimu::SQLAutomator::SQLite3::Step() {
	return sqlite3_step(SQLite3Statement);
}

Reimu::SQLAutomator::SQLite3::~SQLite3() {
#ifdef Reimu_DEBUG
	fprintf(stderr, "[%s @ %p] s3_stmt=%p, s3=%p\n", __PRETTY_FUNCTION__, this, SQLite3Statement, SQLite3DB);
#endif

	if (SQLite3Statement)
		sqlite3_finalize(SQLite3Statement);

	if (SQLite3DB)
		sqlite3_close(SQLite3DB);
}

int Reimu::SQLAutomator::SQLite3::Exec() {
	char *errmsg = NULL;
	int ret = sqlite3_exec(SQLite3DB, Statement.c_str(), Callback, CallbackArg, &errmsg);
	if (errmsg)
		ErrorMessage = std::string(errmsg);
	return ret;
}

int Reimu::SQLAutomator::SQLite3::Exec(std::string stmt_str, int (*callback)(void *, int, char **, char **),void *cbarg) {
	Statement = stmt_str;
	Callback = callback;
	CallbackArg = cbarg;

	return Exec();
}

int Reimu::SQLAutomator::SQLite3::Parse(Reimu::SQLAutomator::StatementType st, std::string table_name,
					std::map<std::string, Reimu::UniversalType> kv) {
	Statement.clear();
	Values.clear();

	if (st & Reimu::SQLAutomator::StatementType::CREATE_TABLE) {

	} else if (st & Reimu::SQLAutomator::StatementType::INSERT_INTO) {
		size_t j = 1;

		Statement += "INSERT INTO " + table_name + " (";

		for (auto const &thisCol : kv) {
			Statement += thisCol.first + ",";

			Values.push_back(thisCol.second);
		}

		Statement.pop_back();
		Statement += ") ";

		Statement += "VALUES (";
		for (auto const &thisCol : kv) {
			Statement += "?" + std::to_string(j) + ",";
			j++;
		}

		Statement.pop_back();
		Statement += ") ";

	} else {
		throw Reimu::Exception(ENOSYS);
	}
}

int Reimu::SQLAutomator::SQLite3::Parse(Reimu::SQLAutomator::StatementType st, std::string table_name) {
	Statement.clear();

	if (st & Reimu::SQLAutomator::StatementType::SELECT_FROM) {
		Statement += "SELECT ";
		if (!Keys.size())
			Statement.push_back('*');
		else {
			for (auto const &thiskey : Keys) {
				Statement += thiskey + ",";
			}
			Statement.pop_back();
		}
		Statement += table_name + " FROM " + table_name;

	} else {
		throw Reimu::Exception(ENOSYS);
	}
}

int Reimu::SQLAutomator::SQLite3::PPB(Reimu::SQLAutomator::StatementType st, std::string table_name,
				      std::map<std::string, Reimu::UniversalType> kv) {
	Parse(st, table_name, kv);
	Prepare();
	Bind();
}

Reimu::UniversalType Reimu::SQLAutomator::SQLite3::Column(int nCol) {
	const char *cType = sqlite3_column_decltype(SQLite3Statement, nCol);

	if (!cType)
		return UniversalType((int64_t)sqlite3_column_int64(SQLite3Statement, nCol));

	if (strstr(cType, "INT"))
		return UniversalType((int64_t)sqlite3_column_int64(SQLite3Statement, nCol));
	else if (strstr(cType, "REAL") || strstr(cType, "FLOA") || strstr(cType, "DOUB"))
		return UniversalType(sqlite3_column_double(SQLite3Statement, nCol));
	else if (strstr(cType, "TEXT") || strstr(cType, "CHAR") || strstr(cType, "CLOB"))
		return UniversalType((char *)sqlite3_column_text(SQLite3Statement, nCol));
	else
		return UniversalType((void *)sqlite3_column_blob(SQLite3Statement, nCol), (size_t)sqlite3_column_bytes(SQLite3Statement, nCol));
}

size_t Reimu::SQLAutomator::SQLite3::ColumnBytes(int nCol) {
	return (size_t)sqlite3_column_bytes(SQLite3Statement, nCol);
}

