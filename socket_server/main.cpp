#include "socket_server.h"
#include <signal.h>

int main(int argc, const char **argv)
{
	SocketServer socketServer;
	socketServer.CreateSocket();
	socketServer.ConnectToServer();
	socketServer.ReceiveMsgOnce();
	//socketServer.CloseSocket();
	return 0;
}