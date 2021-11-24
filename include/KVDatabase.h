//
// Created by ltc on 2021/11/23.
//

#ifndef RAFTKVDATABASE_KVDATABASE_H
#define RAFTKVDATABASE_KVDATABASE_H

#include "KVDatabaseServer.h"

class KVDatabase {
public:
	static void serve();
	static string get(const string& key);
	static string set(const string& key, const string& value);
	static string del(const string& key);
	static void setHandle(const string& cmd);
	static void delHandle(const string& cmd);
private:
	KVDatabase() = default;
	static KVDatabaseServer& getInstance();
private:
	KVDatabaseServer server;
};


#endif //RAFTKVDATABASE_KVDATABASE_H
