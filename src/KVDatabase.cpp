//
// Created by ltc on 2021/11/22.
//

#include "KVDatabase.h"

KVDatabase::KVDatabase() : raft(ObjPool::allocate<Raft>()), server(ObjPool::allocate<TcpServer<Session>>()) {
	raft->addServer(ConfigSystem::getConfig()["kv_database"]["port"].asInt(), IPV4, server);
	raft->registerFuncHandler("set", setHandle);
}

void KVDatabase::serve() {
	raft->serve();
}

void setHandle(const string& cmd) {

}
