//
// Created by root on 17-3-24.
//

#include "SQLAutomator.hpp"

std::string Reimu::SQLAutomator::Statement(int stmt_type) {

	bool InvalidateCaches = 0;

	if (Columns.size() != LastCount_Columns) {
		InvalidateCaches = 1;
	}

	std::string ret;

	if (!InvalidateCaches) {
		Lock_Cache_Statements.lock_shared();
		auto cachetgt = Cache_Statements.find(stmt_type);
		if (cachetgt != Cache_Statements.end()) {
			ret = cachetgt->second;
		}
		Lock_Cache_Statements.unlock_shared();

		if (ret.length())
			return ret;
	}

	std::string sext[2];

	Lock_Statements_Ext.lock_shared();
	auto ftgt_sext = Statements_Ext.find(stmt_type);
	if (ftgt_sext != Statements_Ext.end()) {
		if (ftgt_sext->second[0].size()) {
			sext[0] = ftgt_sext->second[0];
		}
		if (ftgt_sext->second[1].size()) {
			sext[1] = ftgt_sext->second[1];
		}
	}
	Lock_Statements_Ext.unlock_shared();

	if (sext[0].size())
		ret += sext[0];

	if (stmt_type & CREATE_TABLE) {
		ret += "CREATE TABLE " + TableName + " (";

		for (auto &thisCol : ColumnNames) {
			auto thiscspec = Columns.find(thisCol);
			ret += thiscspec->second.ToString() + ",";
		}

		if (sext[1].size())
			ret += sext[1];
		else
			ret.pop_back();
		ret += ") ";
	} else if (stmt_type & INSERT_INTO) {
		ret += "INSERT INTO " + TableName + " VALUES (";

		if (stmt_type & SqlitePrepared) {
			size_t j = 1;
			for (auto const &thisCol : ColumnNames) {
				ret += "?" + std::to_string(j) + ",";
				j++;
			}
		}

		ret.pop_back();
		ret += ") ";

		if (sext[1].size())
			ret += sext[1];
	} else if (stmt_type & SELECT_FROM) {
		ret += "SELECT * FROM " + TableName;
		if (sext[1].size())
			ret += sext[1];
	} else {
		throw Reimu::Exception(ENOSYS);
	}



	Lock_Cache_Statements.lock();
	Cache_Statements.erase(stmt_type);
	Cache_Statements[stmt_type] = ret;
	Lock_Cache_Statements.unlock();

	return ret;
}

void Reimu::SQLAutomator::Statement_Ext(int stmt_type, const char *sext_prefix, const char *sext_suffix) {
	Lock_Statements_Ext.lock();
	if (sext_prefix) {
		Statements_Ext[stmt_type][0].clear();
		Statements_Ext[stmt_type][0] += sext_prefix;
	}
	if (sext_suffix) {
		Statements_Ext[stmt_type][1].clear();
		Statements_Ext[stmt_type][1] += sext_suffix;
	}
	Lock_Statements_Ext.unlock();
}

bool Reimu::SQLAutomator::InsertColumn(Reimu::SQLAutomator::ColumnSpec col) {
	ColumnNames.push_back(col.Name);
	Columns.insert(std::pair<std::string, Reimu::SQLAutomator::ColumnSpec>(col.Name, col));
}

bool Reimu::SQLAutomator::InsertColumns(std::vector<Reimu::SQLAutomator::ColumnSpec> cols) {
	for (auto const &thiscol : cols) {
		InsertColumn(thiscol);
	}
}

//bool Reimu::SQLAutomator::InsertColumns(std::vector<std::pair<std::string, int>> cols) {
//	for (auto const &thiscol : cols) {
//		InsertColumn(ColumnSpec(thiscol.first, (ColumnSpec::Type)thiscol.second));
//	}
//}


Reimu::SQLAutomator::SQLite3 Reimu::SQLAutomator::OpenSQLite3(int flags, char *vfs) {
	return Reimu::SQLAutomator::SQLite3(DatabaseURI, flags, vfs, this);
}

Reimu::SQLAutomator::SQLite3 Reimu::SQLAutomator::OpenSQLite3(int preload_stmt_type, int flags, char *vfs) {
	SQLite3 ret = Reimu::SQLAutomator::SQLite3(DatabaseURI, flags, vfs, this);

	if (preload_stmt_type != -1) {
		ret.Statement = Statement(preload_stmt_type);
	}
}

