//
// Created by ltc on 2021/11/22.
//

#ifndef RAFTKVDATABASE_KVDATABASE_H
#define RAFTKVDATABASE_KVDATABASE_H

#include "raft/include/Raft.h"
#include "net/include/TcpServer.h"
#include "Session.h"

class KVDatabase {
public:
	KVDatabase();
	void serve();
private:
	shared_ptr<Raft> raft;
	shared_ptr<TcpServer<Session>> server;
	unordered_map<string, string> data;
};

static void setHandle(const string& cmd);

#endif //RAFTKVDATABASE_KVDATABASE_H
