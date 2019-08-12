#include "communication.h"

Communication::Communication()
	: flag(0)
{
}

Communication::~Communication()
{
#ifdef DEBUG
	std::cout << "Clean all connections" << std::endl;
#endif
	mOwl.done();
	mOwl.close();
	closesocket(mServer);
	WSACleanup();
}

void Communication::ExitHandler()
{
	flag = 1;
}

int Communication::ConnectToPhaseSpace(const std::string& address)
{
	if(mOwl.open(address) <= 0 | mOwl.initialize() <= 0)
	{
		std::cerr << "[Server] Cannot connect and initialize..." << std::endl;
		std::cerr << "[Server] Check (1) Is the server operating? or ";
		std::cerr << "(2) Is the IP address valid ? or ";
		std::cerr << "(3) Is the cable connected ?" << std::endl;
		return 0;
	}
	std::cout << "[Server] PhaseSpace communication established" << std::endl;
	return 1;
}

void Communication::StartStreaming()
{
	std::cout << "[Server] PhaseSpace server starts streaming." << std::endl;
	mOwl.streaming(1);
	while (mOwl.isOpen() && mOwl.property<int>("initialized") && !flag)
	{
		const OWL::Event *event = mOwl.nextEvent(1000);
		if (!event) continue;

		if (event->type_id() == OWL::Type::ERROR)
		{
			std::cerr << event->name() << ": " << event->str() << std::endl;
		}
		else if (event->type_id() == OWL::Type::FRAME)
		{
			if (event->find("markers", mMarkers) > 0)
			{
				mRecordData.SetCurFrameNum(event->time());
				std::cout << "markers = " << mMarkers.size();
				std::cout << ": " << std::endl;
				mMarkerCount = 0;
				for (OWL::Markers::iterator m = mMarkers.begin(); m != mMarkers.end(); m++)
				{
					if (m->cond > 0) // positive values are good condition values
					{
						SendMsg(ConvertDataToString(
							event->time(), m->id, m->x, m->y, m->z));
						mRecordData.SetCurData(m->id, m->x, m->y, m->z);
						mMarkerCount++;
					}
				} // end for
				std::cout << "[Server] Find " << mMarkerCount << " markers." << std::endl;
				mRecordData.SetDataPerFrame();
			} // end if find markers
		}

	} // end while
}

void Communication::CreateSocket()
{
	int isSocketOK = WSAStartup(MAKEWORD(2, 2), &mWSAData);
	if (isSocketOK != 0)
	{
		std::cerr << "[Socket] Cannot initialize winsock!" << std::endl;
		return;
	}
	mServer = socket(AF_INET, SOCK_STREAM, 0);
	if (mServer == INVALID_SOCKET)
	{
		std::cerr << "[Socket] Cannot create a socket!" << std::endl;
		return;
	}
	std::cout << "[Socket] Socket created" << std::endl;
}

void Communication::BindAndListen()
{
	mServerAddr.sin_family = AF_INET;
	mServerAddr.sin_addr.S_un.S_addr = inet_addr("127.0.1.1");
	mServerAddr.sin_port = htons(5555);
	bind(mServer, (sockaddr*)&mServerAddr, sizeof(mServerAddr));
	listen(mServer, 0);
	std::cout << "[Socket] listening for incoming connections." << std::endl;
	int clientAddrSize = sizeof(mClientAddr);
	if ((mClient = accept(
		mServer, (SOCKADDR*)&mClientAddr, &clientAddrSize)) == INVALID_SOCKET)
	{
		std::cerr << "[Socket] Client not connected!" << std::endl;
	}
	std::cout << "[Socket] Client connected!" << std::endl;
}

std::string Communication::ConvertDataToString(
	const int&frameNum, const int& id, 
	const float& x, const float&y, const float& z)
{
	return std::to_string(frameNum) + ", " + std::to_string(id) + ", " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
}

void Communication::SendMsg(const std::string& data)
{
#ifdef DEBUG
	std::cout << "[Socket] Send message" << std::endl;
#endif
	char buffer[100];
	int result;
	strcpy(buffer, data.c_str());
	result = send(mClient, buffer, sizeof(buffer), 0);
	std::cout << "result not = " << !result << std::endl;
	if (result < 0)
	{
		BindAndListen();
	}
}

void Communication::SendMsgTest()
{
	std::string data;
	char buffer[50];
	int result;
	do {
		data.assign("8, 2447.298828, 1076.380737, -61.742798");
		strcpy(buffer, data.c_str());
		result = send(mClient, buffer, sizeof(buffer), 0);
		if (result > 0)
		{
			memset(buffer, 0, sizeof(buffer));
			data.clear();
		}
		else // listen again for client connection
		{
			BindAndListen();
		}
	} while (!flag);
}

void Communication::CloseSocket()
{
	std::cout << "[Socket] Close socket communication" << std::endl;
	closesocket(mServer);
	WSACleanup();
}