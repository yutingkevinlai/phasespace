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
						mRecordData.SetCurData(m->id, m->x, m->y, m->z);
						mMarkerCount++;
					}
				} // end for
				std::cout << "[Server] Find " << mMarkerCount;
				std::cout << " markers." << std::endl;
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
		std::cerr << "[Client] Cannot initialize winsock!" << std::endl;
		return;
	}
	mServer = socket(AF_INET, SOCK_STREAM, 0);
	if (mServer == INVALID_SOCKET)
	{
		std::cerr << "[Client] Cannot create a socket!" << std::endl;
		return;
	}
	std::cout << "[Client] Socket created" << std::endl;
}

void Communication::ConnectToServer()
{
	mAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // local ip
	mAddr.sin_family = AF_INET;
	mAddr.sin_port = htons(5555);
	connect(mServer, (SOCKADDR*)&mAddr, sizeof(mAddr));
	std::cout << "[Client] Connected to server!" << std::endl;
}

void Communication::SendMsg()
{
	char buffer[4096] = { 'h', 'e', 'l', 'l', 'o' };
	while (true && !flag)
	{
		std::cout << "[send] flag = " << flag << std::endl;
		if (send(mServer, buffer, sizeof(buffer), 0))
		{
		}
	}

}

void Communication::CloseSocketComm()
{
	std::cout << "[Client] Close socket communication" << std::endl;
	closesocket(mServer);
	WSACleanup();
}