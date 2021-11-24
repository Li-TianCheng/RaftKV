//
// Created by ltc on 2021/11/22.
//

#include "Session.h"

Session::Session(int bufferChunkSize) : TcpSession(bufferChunkSize) {

}

void Session::handleReadDone(iter pos, size_t n) {
	for (size_t i = 0; i < n; i++) {
		cmd += *pos++;
	}
	if (cmd.size() > 2 && cmd[cmd.size()-2] == '\r' && cmd[cmd.size()-1] == '\n') {
		vector<string> split = utils::split(cmd.substr(0, cmd.size()-2), ' ');
		if (split.size() > 1) {
			shared_ptr<string> reply = ObjPool::allocate<string>();
			if (split[0] == "GET") {
				if (split.size() == 2) {
					*reply = KVDatabase::get(split[1]);
				} else {
					*reply = "INPUT ERROR";
				}
			}
			if (split[0] == "SET") {
				if (split.size() == 3) {
					*reply = KVDatabase::set(split[1], split[2]);
				} else {
					*reply = "INPUT ERROR";
				}
			}
			if (split[0] == "DELETE") {
				if (split.size() == 2) {
					*reply = KVDatabase::del(split[1]);
				} else {
					*reply = "INPUT ERROR";
				}
			}
			*reply += "\r\n";
			write(reply);
		} else {
			mutex->lock();
			response = std::move(split[0]);
			closeConnection();
		}
	}
	readDone(n);
}

void Session::sessionInit() {
	if (send != nullptr) {
		write(send);
	}
}

void Session::sessionClear() {
	if (mutex != nullptr) {
		condition->notifyAll(*mutex);
	}
}
