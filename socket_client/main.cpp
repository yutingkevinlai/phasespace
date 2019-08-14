#include "socket_client.h"
#include <signal.h>

int main(int argc, const char **argv)
{
	SocketClient socketClient;
	socketClient.CreateSocket();
	socketClient.ConnectToServer();
	socketClient.ReceiveMsgOnce();
	//socketClient.CloseSocket();
	return 0;
}