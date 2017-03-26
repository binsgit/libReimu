//
// Created by root on 17-3-24.
//

#include "SQLAutomator.hpp"

std::string Reimu::SQLAutomator::Statement(Reimu::SQLAutomator::StatmentType st) {

	bool InvalidateCaches = 0;

	if (Columns.size() != LastCount_Columns) {
		InvalidateCaches = 1;
	}

	std::string ret;

	if (!InvalidateCaches) {
		Lock_Cache_Statements.lock_shared();
		auto cachetgt = Cache_Statements.find(st);
		if (cachetgt != Cache_Statements.end()) {
			ret = cachetgt->second;
		}
		Lock_Cache_Statements.unlock_shared();
		return ret;
	}

	std::string sext[2];

	Lock_Statements_Ext.lock_shared();
	auto ftgt_sext = Statements_Ext.find(st);
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

	if (st & CREATE_TABLE) {
		ret += "CREATE TABLE " + TableName + " (";

		for (auto const &thisCol : Columns) {
			ret += thisCol.Name + " " + thisCol.ToString(thisCol.DataType) + ",";
		}

		ret.pop_back();
		ret += ") ";
	} else if (st & INSERT_INTO) {
		ret += "INSERT INTO " + TableName + " VALUES (";

		if (st & SqlitePrepared) {
			size_t j = 1;
			for (auto const &thisCol : Columns) {
				ret += "?" + std::to_string(j) + ",";
				j++;
			}
		}

		ret.pop_back();
		ret += ") ";
	} else {
		throw Reimu::Exception(ENOSYS);
	}

	if (sext[1].size())
		ret += sext[1];

	Lock_Cache_Statements.lock();
	Cache_Statements.erase(st);
	Cache_Statements[st] = ret;
	Lock_Cache_Statements.unlock();

	return ret;
}

void Reimu::SQLAutomator::Statement_Ext(Reimu::SQLAutomator::StatmentType st, const char *sext_prefix, const char *sext_suffix) {
	Lock_Statements_Ext.lock();
	if (sext_prefix) {
		if (sext_prefix[0]) {
			Statements_Ext[st][0].clear();
			Statements_Ext[st][0] += sext_prefix;
		}
	}
	if (sext_prefix) {
		if (sext_prefix[0]) {
			Statements_Ext[st][1].clear();
			Statements_Ext[st][1] += sext_suffix;
		}
	}
	Lock_Statements_Ext.unlock_shared();
}
