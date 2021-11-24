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
	auto mutex = ObjPool::allocate<Mutex>();
	auto condition = ObjPool::allocate<Condition>();
	mutex->lock();
	auto session = server->getSession();
	shared_ptr<string> send = ObjPool::allocate<string>();
	*send = "GET "+key+"\r\n";
	static_pointer_cast<Session>(session)->send = send;
	static_pointer_cast<Session>(session)->mutex = mutex;
	static_pointer_cast<Session>(session)->condition = condition;
	raft->getListener()->addNewSession(session, re+":"+ to_string(port), IPV4);
	condition->wait(*mutex);
	re = std::move(static_pointer_cast<Session>(session)->response);
	mutex->unlock();
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
		auto mutex = ObjPool::allocate<Mutex>();
		auto condition = ObjPool::allocate<Condition>();
		mutex->lock();
		auto session = server->getSession();
		shared_ptr<string> send = ObjPool::allocate<string>();
		*send = "SET "+key+" "+value+"\r\n";
		static_pointer_cast<Session>(session)->send = send;
		static_pointer_cast<Session>(session)->mutex = mutex;
		static_pointer_cast<Session>(session)->condition = condition;
		raft->getListener()->addNewSession(session, re+":"+ to_string(port), IPV4);
		condition->wait(*mutex);
		re = std::move(static_pointer_cast<Session>(session)->response);
		mutex->unlock();
	}
	if (re.empty()) {
		re = "RETRY";
	}
	return re;
}
