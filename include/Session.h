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
	void handleReadDone(iter pos, size_t n) override;
private:
	string cmd;
};


#endif //RAFTKVDATABASE_SESSION_H
