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
	// create phasespace thread
	if (gCommunication.ConnectToPhaseSpace(address))
	{
		gCommunication.StartStreaming();
	}
	else
	{
		exit(-1);
	}
}

void SocketHostStreaming()
{
	// create socket thread
	gCommunication.CreateSocket();
	gCommunication.BindAndListen();
	do {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		gCommunication.SendMsg();
	} while (!gCommunication.flag);
}

void SocketTargetStreaming()
{
	
}

int main(int argc, const char **argv)
{
	// read PhaseSpace IP address
	std::string phasespaceIP;
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(0);
	std::cin >> phasespaceIP;

	signal(SIGINT, ExitHandler);

	// TCP socket to host threading
	std::thread socketHostThread(SocketHostStreaming);

	// UDP socket to target threading
	std::thread socketTargetThread(SocketTargetStreaming);

	// PhaseSpace streaming
	std::thread phasespaceThread(PhaseSpaceStreaming, phasespaceIP);
	
	phasespaceThread.join();
	socketHostThread.join();
	socketTargetThread.join();
	gCommunication.CloseSocket();
	return 0;
}