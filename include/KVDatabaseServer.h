//
// Created by ltc on 2021/11/22.
//

#ifndef RAFTKVDATABASE_KVDATABASESERVER_H
#define RAFTKVDATABASE_KVDATABASESERVER_H

#include "raft/include/Raft.h"
#include "net/include/TcpServer.h"

class Session;

class KVDatabaseServer {
public:
	KVDatabaseServer();
	string get(const string& key);
	string set(const string& key, const string& value);
	void serve();
private:
	friend class KVDatabase;
private:
	shared_ptr<Raft> raft;
	shared_ptr<TcpServer<Session>> server;
	unordered_map<string, string> data;
};

#endif //RAFTKVDATABASE_KVDATABASESERVER_H
