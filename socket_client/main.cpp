#include "socket_client.h"
#include <signal.h>

#define OFFLINE

int main(int argc, const char **argv)
{
	SocketClient socketClient;
	socketClient.CreateSocket();
	socketClient.ConnectToServer();
#ifdef OFFLINE
	socketClient.ReceiveMsg();
#else
	socketClient.ReceiveMsgOnce();
#endif
	return 0;
}