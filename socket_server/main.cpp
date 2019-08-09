#include "socket_server.h"
#include <signal.h>

int gFlag = 0;

void ExitHandler(int s)
{
	gFlag = 1;
}

int main(int argc, const char **argv)
{
	signal(SIGINT, ExitHandler);
	SocketServer socketServer;
	socketServer.CreateSocket();
	socketServer.BindAndListen();
	socketServer.ReceiveMsg();
	//socketServer.CloseSocket();
	return 0;
}