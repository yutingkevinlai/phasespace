#include <signal.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <owl/owl.hpp>
#include "record_data.h"
#include "communication.h"

using namespace std;

int gFlag = 0;
Communication gCommunication;

void ExitHandler(int s)
{
	gCommunication.flag = 1;
	std::cout << "flag = " << gCommunication.flag << std::endl;
}

int main(int argc, const char **argv)
{
	if (argc <= 1)
	{
		cout << "[Main] Please provide a host IP address" << endl;
		return 0;
	}

	signal(SIGINT, ExitHandler);

	// Connect to server first
	gCommunication.CreateSocket();
	gCommunication.ConnectToServer();		
	gCommunication.SendMsg();

	string address = argv[1];
#ifdef ALLOW_PHASESPACE
	if (gCommunication.ConnectToPhaseSpace(address))
	{
		gCommunication.StartStreaming();
	}
#endif

	//gCommunication.CloseSocketComm();
	return 0;
}