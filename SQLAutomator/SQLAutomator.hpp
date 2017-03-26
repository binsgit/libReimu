//
// Created by root on 17-3-24.
//

#ifndef LIBREIMU_SQLAUTOMATOR_HPP
#define LIBREIMU_SQLAUTOMATOR_HPP

#include "../libReimu.hpp"
#include "../Exception/Exception.hpp"

namespace Reimu {
    class SQLAutomator {
    private:
	size_t LastCount_Columns = 0;
	std::unordered_map<int, std::string> Cache_Statements;

    public:
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

	enum StatmentType {
	    CREATE_TABLE = 0x1, INSERT_INTO = 0x2,

	    SqlitePrepared = 0x10000
	};

	std::set<Reimu::SQLAutomator::ColumnSpec> Columns;

	std::string Statement(Reimu::SQLAutomator::StatmentType st);

    };
}

#endif //LIBREIMU_SQLAUTOMATOR_HPP
