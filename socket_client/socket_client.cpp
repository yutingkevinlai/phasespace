#include "socket_client.h"

API SocketClient::SocketClient()
{
}

API SocketClient::~SocketClient()
{
	closesocket(mClient);
	WSACleanup();
	std::cout << "[Socket] Socket closed." << std::endl;
}

API void SocketClient::CreateSocket()
{
	int isSocketOK = WSAStartup(MAKEWORD(2, 2), &mWSAData);
	if (isSocketOK != 0)
	{
		std::cerr << "[Client] Cannot initialize socket!" << std::endl;
		return;
	}
	mServer = socket(AF_INET, SOCK_STREAM, 0);
	if (mServer == INVALID_SOCKET)
	{
		std::cerr << "[Client] Cannot create a socket!" << std::endl;
		return;
	}
	std::cout << "[Client] Socket created." << std::endl;
}

API void SocketClient::ConnectToServer()
{
	mAddr.sin_addr.s_addr = inet_addr("127.0.1.1"); // local ip
	mAddr.sin_family = AF_INET;
	mAddr.sin_port = htons(5555);
	connect(mServer, (SOCKADDR*)&mAddr, sizeof(mAddr));
	std::cout << "[Client] Connected to server!" << std::endl;
}

API void SocketClient::ReceiveMsg()
{
	char buffer[100];
	int result;
	do {
		result = recv(mServer, buffer, sizeof(buffer), 0);
		if (result > 0)
		{
			std::cout << "[Client] Received data: " << buffer << std::endl;
			mRecordData.SetCurData(buffer);
		}
		else if (result == 0)
		{
			std::cout << "[Client] Server closed." << std::endl;
			CloseSocket();
			break;
		}
		else
		{
			std::cerr << "[Client] Receive data failed. Closing." << std::endl;
			CloseSocket();
			break;
		}
		memset(buffer, 0, sizeof(buffer));
	} while (result > 0);
}

API void SocketClient::ReceiveMsgOnce()
{
	char buffer[100];
	int result;
	result = recv(mServer, buffer, sizeof(buffer), 0);
	if (result > 0)
	{
		std::cout << "[Client] Received data: " << buffer << std::endl;
	}
	else if (result == 0)
	{
		std::cout << "[Client] Server closed." << std::endl;
		CloseSocket();
	}
	else
	{
		std::cerr << "[Client] Receive data failed. Closing." << std::endl;
		CloseSocket();
	}
}

API void SocketClient::CloseSocket()
{
	closesocket(mClient);
	WSACleanup();
	std::cout << "[Client] Socket closed" << std::endl;
}