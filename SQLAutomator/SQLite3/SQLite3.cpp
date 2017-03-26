#include "../SQLAutomator.hpp"

Reimu::SQLAutomator::SQLite3::SQLite3() {

}

Reimu::SQLAutomator::SQLite3::SQLite3(std::string db_uri, int flags, char *vfs) {
	if (SQLITE_OK != Open(db_uri, flags, vfs))
		throw Reimu::Exception(EBADF);
}

int Reimu::SQLAutomator::SQLite3::Open(std::string db_uri, int flags, char *vfs) {
	return sqlite3_open_v2(db_uri.c_str(), &SQLite3DB, flags, (const char *)vfs);
}


int Reimu::SQLAutomator::SQLite3::Bind(size_t narg, Reimu::UniversalType thisval) {
	if (thisval.Type < 10) {
		return sqlite3_bind_int64(SQLite3Statement, (int)narg, thisval.Int64());
	} else if (thisval.Type > 10 && thisval.Type < 20) {
		return sqlite3_bind_double(SQLite3Statement, (int)narg, thisval.Double());
	} else if (thisval.Type == Reimu::UniversalType::Types::STRING) {
		return sqlite3_bind_text(SQLite3Statement, (int)narg, thisval.String().c_str(), (int)thisval.String().size(), SQLITE_STATIC);
	} else if (thisval.Type == Reimu::UniversalType::Types::BLOB) {
		return sqlite3_bind_blob64(SQLite3Statement, (int)narg, &thisval.Blob()[0], thisval.Blob().size(), SQLITE_STATIC);
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
	return sqlite3_prepare(SQLite3DB, Statement.c_str(), (int)Statement.length(), &SQLite3Statement, NULL);
}


int Reimu::SQLAutomator::SQLite3::Reset() {
	return sqlite3_reset(SQLite3Statement);
}

int Reimu::SQLAutomator::SQLite3::Step() {
	return sqlite3_step(SQLite3Statement);
}

Reimu::SQLAutomator::SQLite3::~SQLite3() {
	if (SQLite3Statement)
		sqlite3_finalize(SQLite3Statement);

	if (SQLite3DB)
		sqlite3_close(SQLite3DB);
}

int Reimu::SQLAutomator::SQLite3::Exec(std::string stmt_str, int (*callback)(void *, int, char **, char **),void *cbarg) {
	char *errmsg = NULL;
	int ret = sqlite3_exec(SQLite3DB, stmt_str.c_str(), callback, cbarg, &errmsg);
	if (errmsg)
		ErrorMessage = std::string(errmsg);
	return ret;
}

int Reimu::SQLAutomator::SQLite3::Parse(Reimu::SQLAutomator::StatmentType st, std::string table_name,
					std::map<std::string, Reimu::UniversalType> kv) {
	Statement.clear();
	Values.clear();

	if (st & Reimu::SQLAutomator::StatmentType::CREATE_TABLE) {

	} else if (st & Reimu::SQLAutomator::StatmentType::INSERT_INTO) {
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

int Reimu::SQLAutomator::SQLite3::Parse(Reimu::SQLAutomator::StatmentType st, std::string table_name) {
	Statement.clear();

	if (st & Reimu::SQLAutomator::StatmentType::SELECT_FROM) {
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

