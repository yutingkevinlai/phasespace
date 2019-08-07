#include "server_communication.h"
#include <owl/owl.hpp>

ServerCommunication::ServerCommunication(const string& address)
	: mFlag(0)
{
	if (ConnectToServer(address))
	{
		StartStreaming();
	}
}

ServerCommunication::~ServerCommunication()
{
	mOwl.done();
	mOwl.close();
}

void ServerCommunication::ExitHandler(int s)
{
	mFlag = 1;
}

int ServerCommunication::ConnectToServer(const string& address)
{
	if(mOwl.open(address) <= 0 | mOwl.initialize() <= 0)
	{
		cerr << "[Server] Cannot connect and initialize..." << endl;
		cerr << "[Server] Check (1) Is the server operating? or ";
		cerr << "(2) Is the IP address valid ? or ";
		cerr << "(3) Is the cable connected ?" << endl;
		return 0;
	}
	cout << "[Server] Communication established" << endl;
	return 1;
}

void ServerCommunication::StartStreaming()
{
	mOwl.streaming(1);
	while (mOwl.isOpen() && mOwl.property<int>("initialized") && !mFlag)
	{
		const OWL::Event *event = mOwl.nextEvent(1000);
		if (!event) continue;

		if (event->type_id() == OWL::Type::ERROR)
		{
			cerr << event->name() << ": " << event->str() << endl;
		}
		else if (event->type_id() == OWL::Type::FRAME)
		{
			if (event->find("markers", mMarkers) > 0)
			{
				mRecordData.SetCurFrameNum(event->time()); // set the frame number for recording
				cout << "markers = " << mMarkers.size() << ": " << endl;
				mMarkerCount = 0;
				for (OWL::Markers::iterator m = mMarkers.begin(); m != mMarkers.end(); m++)
				{
					if (m->cond > 0) // positive values are good condition values
					{
						mRecordData.SetCurData(m->id, m->x, m->y, m->z);
						//cout << " " << m->id << ") frame = " << m->time  << " "; 
						//cout << "x = "<< m->x << ", y = " << m->y << ", z = " << m->z << endl;
						mMarkerCount++;
					}
				} // end for
				cout << "[Main] Find " << mMarkerCount << " markers." << endl;
				mRecordData.SetDataPerFrame();
			} // end if find markers
		}

	} // end while
}

