#include <signal.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>
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

void PhaseSpaceStreaming(const std::string& address)
{
	std::cout << "Create phasespace thread" << std::endl;
	if (gCommunication.ConnectToPhaseSpace(address))
	{
		gCommunication.StartStreaming();
	}
}

void SocketStreaming()
{
	std::cout << "Create socket thread" << std::endl;
	gCommunication.CreateSocket();
	gCommunication.BindAndListen();
	do {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		gCommunication.SendMsg();
	} while (!gCommunication.flag);
}

int main(int argc, const char **argv)
{
	if (argc <= 1)
	{
		cout << "[Main] Please provide a host IP address" << endl;
		return 0;
	}

	signal(SIGINT, ExitHandler);

	// Establish socket communication first
	std::thread socketThread(SocketStreaming);

	// PhaseSpace streaming
#ifdef ALLOW_PHASESPACE
	string address = argv[1];
	std::thread phasespaceThread(PhaseSpaceStreaming, address);
	//PhaseSpaceStreaming(address);
	phasespaceThread.join();
#endif

	socketThread.join();
	gCommunication.CloseSocket();
	return 0;
}