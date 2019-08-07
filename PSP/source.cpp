#include <signal.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <owl/owl.hpp>
#include "record_data.h"
#include "server_communication.h"

using namespace std;

int gFlag = 0;

void ExitHandler(int s)
{
	gFlag = 1;
}

int main(int argc, const char **argv)
{
	if (argc <= 1)
	{
		cout << "[Main] Please provide a host IP address" << endl;
		return 0;
	}

	// create Ctrl+C handler, exit and save data upon pressing Ctrl+C
	signal(SIGINT, ExitHandler);

	string address = argv[1];
	ServerCommunication serverCommunication(address);

	return 0;
}