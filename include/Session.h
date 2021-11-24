//
// Created by ltc on 2021/11/22.
//

#ifndef RAFTKVDATABASE_SESSION_H
#define RAFTKVDATABASE_SESSION_H

#include "net/include/TcpSession.h"
#include "utils/include/StringUtils.h"
#include "KVDatabase.h"

class Session : public TcpSession {
public:
	explicit Session(int bufferChunkSize);
	void sessionInit() override;
	void sessionClear() override;
	void handleReadDone(iter pos, size_t n) override;
private:
	friend class KVDatabaseServer;
private:
	shared_ptr<string> send;
	string cmd;
	string response;
	shared_ptr<Mutex> mutex;
	shared_ptr<Condition> condition;
};


#endif //RAFTKVDATABASE_SESSION_H
