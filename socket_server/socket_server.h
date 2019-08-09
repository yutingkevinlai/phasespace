#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H
#pragma once

#include <winsock2.h>
#include <iostream>
#include <signal.h>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)

//#define ALLOW_SERVER

class SocketServer
{
public:
	SocketServer();
	~SocketServer();

	// socket communication
	void CreateSocket();
	void BindAndListen();
	void ReceiveMsg();
	void CloseSocket();

private:
	WSADATA mWSAData;
	SOCKET mServer;
	SOCKET mClient;
	SOCKADDR_IN mServerAddr;
	SOCKADDR_IN mClientAddr;
};

#endif // !SOCKET_SERVER_H
