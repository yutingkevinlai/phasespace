#include "socket_server.h"

SocketServer::SocketServer()
{
}

SocketServer::~SocketServer()
{
	closesocket(mClient);
	WSACleanup();
}

void SocketServer::CreateSocket()
{
	int isSocketOK = WSAStartup(MAKEWORD(2, 2), &mWSAData);
	if (isSocketOK != 0)
	{
		std::cerr << "[Socket] Cannot initialize socket!" << std::endl;
		return;
	}
	mServer = socket(AF_INET, SOCK_STREAM, 0);
	if (mServer == INVALID_SOCKET)
	{
		std::cerr << "[Socket] Cannot create a socket!" << std::endl;
		return;
	}
	std::cout << "[Socket] Socket created" << std::endl;
}

void SocketServer::BindAndListen()
{
	mServerAddr.sin_family = AF_INET;
	mServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	mServerAddr.sin_port = htons(5555);
	bind(mServer, (sockaddr*)&mServerAddr, sizeof(mServerAddr));
	listen(mServer, 0);
	std::cout << "[Socket] listening for incoming connections" << std::endl;
}

void SocketServer::ReceiveMsg()
{
	char buffer[4096];
	int clientAddrSize = sizeof(mClientAddr);
	if ((mClient = accept(
		mServer, (SOCKADDR*)&mClientAddr, &clientAddrSize)) == INVALID_SOCKET)
	{
		std::cerr << "[Socket] Client not connected!" << std::endl;
	}
	std::cout << "[Socket] Client connected!" << std::endl;

	while (true)
	{
		if (recv(mClient, buffer, sizeof(buffer), 0))
		{
			std::cout << "[Socket] Client says: " << buffer << std::endl;
		}
		memset(buffer, 0, sizeof(buffer));
	}
}

void SocketServer::CloseSocket()
{
	closesocket(mClient);
	WSACleanup();
	std::cout << "[Socket] Socket closed" << std::endl;
}