//
// Created by ltc on 2021/11/22.
//

#include "KVDatabaseServer.h"
#include "KVDatabase.h"
#include "Session.h"

KVDatabaseServer::KVDatabaseServer() : raft(ObjPool::allocate<Raft>()), server(ObjPool::allocate<TcpServer<Session>>()) {
	raft->addServer(ConfigSystem::getConfig()["kv_database"]["port"].asInt(), IPV4, server);
	raft->registerFuncHandler("SET", KVDatabase::setHandle);
}

void KVDatabaseServer::serve() {
	raft->serve();
}

string KVDatabaseServer::get(const string &key) {
	auto re = raft->isLeader();
	if (re.empty()) {
		if (data.find(key) == data.end()) {
			return "NULL";
		}
		return data[key];
	}
	return re;
	// TODO 自动转发
}

string KVDatabaseServer::set(const string &key, const string& value) {
	auto re = raft->startCmd("SET", key+" "+value);
	if (re == "success") {
		return value;
	}
	if (re == "failed") {
		return "ERROR";
	}
	return re;
	// TODO 自动转发
}
