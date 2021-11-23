//
// Created by ltc on 2021/11/22.
//

#include "Session.h"

Session::Session(int bufferChunkSize) : TcpSession(bufferChunkSize) {

}

void Session::handleReadDone(iter pos, size_t n) {
	TcpSession::handleReadDone(pos, n);
}
