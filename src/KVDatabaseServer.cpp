//
// Created by ltc on 2021/11/22.
//

#include "KVDatabaseServer.h"
#include "KVDatabase.h"
#include "Session.h"

KVDatabaseServer::KVDatabaseServer() : port(ConfigSystem::getConfig()["kv_database"]["port"].asInt()),
									   raft(ObjPool::allocate<Raft>()), server(ObjPool::allocate<TcpServer<Session>>()) {
	raft->getListener()->registerListener(port, IPV4, server);
	raft->registerFuncHandler("SET", KVDatabase::setHandle);
	raft->registerFuncHandler("DELETE", KVDatabase::delHandle);
	raft->registerGenSnapshotHandler(KVDatabase::genSnapshotHandler);
	raft->registerInstallSnapshotHandler(KVDatabase::installSnapshotHandler);
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
	re = forward(re+":"+ to_string(port), "GET "+key);
	if (re.empty()) {
		re = "RETRY";
	}
	return re;
}

string KVDatabaseServer::set(const string &key, const string& value) {
	auto re = raft->startCmd("SET", key+" "+value);
	if (re == "success") {
		return value;
	}
	if (re == "failed") {
		return "ERROR";
	}
	if (!re.empty()) {
		re = forward(re+":"+ to_string(port), "SET "+key+" "+value);
	}
	if (re.empty()) {
		re = "RETRY";
	}
	return re;
}

string KVDatabaseServer::del(const string &key) {
	auto re = raft->startCmd("DELETE", key);
	if (re == "success") {
		return key;
	}
	if (re == "failed") {
		return "ERROR";
	}
	if (!re.empty()) {
		re = forward(re+":"+ to_string(port), "DELETE "+key);
	}
	if (re.empty()) {
		re = "RETRY";
	}
	return re;
}

string KVDatabaseServer::forward(const string &address, const string &cmd) {
	LOG(Info, "forward["+address+"] "+cmd);
	auto mutex = ObjPool::allocate<Mutex>();
	auto condition = ObjPool::allocate<Condition>();
	mutex->lock();
	auto session = server->getSession();
	shared_ptr<string> send = ObjPool::allocate<string>();
	*send = cmd+"\r\n";
	static_pointer_cast<Session>(session)->send = send;
	static_pointer_cast<Session>(session)->mutex = mutex;
	static_pointer_cast<Session>(session)->condition = condition;
	if (raft->getListener()->addNewSession(session, address, IPV4)) {
		condition->wait(*mutex);
	}
	mutex->unlock();
	return static_pointer_cast<Session>(session)->response;
}
