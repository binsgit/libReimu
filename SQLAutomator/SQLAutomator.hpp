//
// Created by root on 17-3-24.
//

#ifndef LIBREIMU_SQLAUTOMATOR_HPP
#define LIBREIMU_SQLAUTOMATOR_HPP

#include "../libReimu.hpp"
#include "../Exception/Exception.hpp"
#include "../UniversalType/UniversalType.hpp"

#include <sqlite3.h>

namespace Reimu {
    class SQLAutomator {
    private:
	size_t LastCount_Columns = 0;
	std::unordered_map<int, std::string> Cache_Statements;
	std::shared_timed_mutex Lock_Cache_Statements;

	std::unordered_map<int, std::string[2]> Statements_Ext;
	std::shared_timed_mutex Lock_Statements_Ext;

    public:

	enum StatmentType {
	    CREATE_TABLE = 0x1, INSERT_INTO = 0x2, SELECT_FROM = 0x3,

	    SqlitePrepared = 0x10000
	};


	class ColumnSpec {
	private:
	    std::string Cache_S;

	public:
	    enum _DataType {
		INTEGER = 0x1, TEXT = 0x2, REAL = 0x4, BLOB = 0x8, // sqlite

		UNSIGNED = 0x1000,
		NOT_NULL = 0x10000, UNIQUE = 0x20000, PRIMARY_KEY = 0x40000, AUTOINCREMENT = 0x80000 //
	    };

	    bool const operator== (const Reimu::SQLAutomator::ColumnSpec &o) const;
	    bool const operator< (const Reimu::SQLAutomator::ColumnSpec &o) const;

	    size_t Length = 0;
	    std::string Name;
	    _DataType DataType;

	    ColumnSpec(std::string Name, _DataType dataType, size_t Length=0);

	    static std::string ToString(Reimu::SQLAutomator::ColumnSpec::_DataType dt);
	    std::string ToString();
	};

	class SQLite3 {
	public:
	    sqlite3 *SQLite3DB = NULL;
	    sqlite3_stmt *SQLite3Statement = NULL;

	    std::string Statement;
	    std::vector<std::string> Keys;
	    std::vector<Reimu::UniversalType> Values;

	    std::string ErrorMessage;

	    SQLite3();
	    SQLite3(std::string db_uri, int flags=SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_READWRITE, char *vfs = NULL);

	    int Open(std::string db_uri, int flags=SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_READWRITE, char *vfs = NULL);

	    int Exec(std::string stmt_str, int (*callback)(void *, int, char **, char **) = NULL, void *cbarg = NULL);
	    int Prepare();

	    int Parse(Reimu::SQLAutomator::StatmentType st, std::string table_name);

	    int Parse(Reimu::SQLAutomator::StatmentType st, std::string table_name,
		      std::map<std::string, Reimu::UniversalType> kv);

	    void Bind();
	    int Bind(size_t narg, Reimu::UniversalType thisval);

	    int Step();
	    int Reset();

	    ~SQLite3();
	};



	std::string TableName;

	std::set<Reimu::SQLAutomator::ColumnSpec> Columns;

	std::string Statement(Reimu::SQLAutomator::StatmentType st);
//	std::string Statement(Reimu::SQLAutomator::StatmentType st, std::vector<std::string> args);


	void Statement_Ext(Reimu::SQLAutomator::StatmentType st, const char *sext_prefix, const char *sext_suffix);

    };
}

#endif //LIBREIMU_SQLAUTOMATOR_HPP
