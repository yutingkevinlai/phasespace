#ifndef SERVER_COMMUNICATION_H
#define SERVER_COMMUNICATION_H
#pragma once

#include <owl/owl.hpp>
#include <iostream>
#include <signal.h>
#include "record_data.h"

using namespace std;

class ServerCommunication
{
public: 
	ServerCommunication(const string& address);
	~ServerCommunication();
	void ExitHandler(int s);
	int ConnectToServer(const string& address);
	void StartStreaming();

private:
	int mMarkerCount;
	int mFlag;
	RecordData mRecordData;
	OWL::Context mOwl;
	OWL::Markers mMarkers;

};


#endif // !SERVER_COMMUNICATION_H
