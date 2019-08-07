#include <signal.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <owl/owl.hpp>
#include "record_data.h"

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
	string address = argv[1];
	OWL::Context owl;
	OWL::Markers markers;
	RecordData recordData;

	// check owl validity
	if (owl.open(address) <= 0 || owl.initialize() <= 0)
	{
		cerr << "[Main] Cannot connect and initialize..." << endl;
		cerr << "[Main] Check (1) Is the server operating? or ";
		cerr << "(2) Is the IP address valid ? or (3) Is the cable connected ? " << endl;
		return 0;
	}
	cout << "[Main] Connection established" << endl;

	// create Ctrl+C handler, exit and save data upon pressing Ctrl+C
	signal(SIGINT, ExitHandler);

	// start streaming
	owl.streaming(1);

	int markerCount;
	// main loop
	while (owl.isOpen() && owl.property<int>("initialized") && !gFlag)
	{
		const OWL::Event *event = owl.nextEvent(1000);
		if (!event) continue;
		
		if (event->type_id() == OWL::Type::ERROR) 
		{
			cerr << event->name() << ": " << event->str() << endl;
		}
		else if (event->type_id() == OWL::Type::FRAME)
		{
			//cout << "time = " << event->time() << " " << event->type_name() << " ";
			//cout << event->name() << " = " << event->size<OWL::Event>() << ": " << endl;

			if (event->find("markers", markers) > 0)
			{
				recordData.SetCurFrameNum(event->time()); // set the frame number for recording
				cout << "markers = " << markers.size() << ": " << endl;
				markerCount = 0;
				for (OWL::Markers::iterator m = markers.begin(); m != markers.end(); m++)
				{
					if (m->cond > 0) // positive values are good condition values
					{
						recordData.SetCurData(m->id, m->x, m->y, m->z);
						//cout << " " << m->id << ") frame = " << m->time  << " "; 
						//cout << "x = "<< m->x << ", y = " << m->y << ", z = " << m->z << endl;
						markerCount++;
					}
				} // end for
				cout << "[Main] Find " << markerCount << " markers." << endl;
				recordData.SetDataPerFrame();
			} // end if find markers
		}
	} // end while

	owl.done();
	owl.close();
	return 0;
}