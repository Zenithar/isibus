/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2007                                                    *
 *   IsiBus 2007                  					   *
 ***************************************************************************/
 
#ifndef __BUS_POSITIONMSG__
#define __BUS_POSITIONMSG__

#include <Ivycpp.h>
#include <IvyApplication.h>

namespace isibus 
{
	class ControlCenter;
	
	namespace msg 
	{
		class BusPositionMsg : public IvyMessageCallback {
			public:
				BusPositionMsg(ControlCenter* cc):m_cc(cc) {}
			
				void OnMessage(IvyApplication *app, int argc, const char **argv)
				{
					cout << "[GPS] Bus[" << argv[0] << "] line: " << argv[1] << " pos:(" << argv[2] << ":" << argv[3] << ") capacity: " << argv[4] << " speed: " << argv[5] <<  endl;
					
					Bus* temp = m_cc->m_RoadMap->getBusList()[atoi(argv[0])];
					
					pthread_mutex_lock(&verrou);
					
					temp->setLine(atoi(argv[1]));
					temp->setRoadID(atoi(argv[2]));
					temp->setRoadDistance(atoi(argv[3]));
					temp->setCapacity(atoi(argv[4]));
					temp->setSpeed(atoi(argv[5]));
					
					pthread_mutex_unlock(&verrou);
				}
			
			private:
				ControlCenter* m_cc;
				pthread_mutex_t verrou;
		};
	}
}

#endif //__BUS_POSITIONMSG__
