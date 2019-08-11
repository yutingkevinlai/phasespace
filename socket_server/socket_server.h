#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H


#define API __declspec(dllexport)
#pragma once

#include <winsock2.h>
#include <iostream>
#include <signal.h>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)

//#define ALLOW_SERVER

class API SocketServer
{
public:
	SocketServer();
	~SocketServer();

	// socket communication
	void CreateSocket();
	void BindAndListen();
	void ConnectToServer();
	void ReceiveMsg();
	void CloseSocket();

private:
	WSADATA mWSAData;
	SOCKET mServer;
	SOCKET mClient;
	SOCKADDR_IN mServerAddr;
	SOCKADDR_IN mClientAddr;
	SOCKADDR_IN mAddr;
};

#endif // !SOCKET_SERVER_H
