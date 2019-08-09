#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#pragma once

#include <winsock2.h>
#include <owl/owl.hpp>
#include <iostream>
#include <signal.h>
#include "record_data.h"

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)
//#define ALLOW_PHASESPACE
#define DEBUG

class Communication
{
public: 
	Communication();
	~Communication();
	void ExitHandler();

	// handling PhaseSpace data
	int ConnectToPhaseSpace(const std::string& address);
	void StartStreaming();
	std::string SetMsg();
	
	// socket communication
	void CreateSocket();
	void ConnectToServer();
	void SendMsg();
	void CloseSocketComm();

	int flag;
private:
	int mMarkerCount;
	RecordData mRecordData;
	OWL::Context mOwl;
	OWL::Markers mMarkers;
	WSADATA mWSAData;
	SOCKET mServer;
	SOCKADDR_IN mAddr;
};

#endif // !COMMUNICATION_H
