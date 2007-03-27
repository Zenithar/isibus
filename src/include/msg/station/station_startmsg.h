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
 
#ifndef __STATION_STARTMSG__
#define __STATION_STARTMSG__

#include <Ivycpp.h>
#include <IvyApplication.h>

namespace isibus 
{
	class ControlCenter;
	
	namespace msg 
	{
		class StationStartMsg : public IvyMessageCallback {
			public:
				StationStartMsg(ControlCenter* cc):m_cc(cc) {}
			
				void OnMessage(IvyApplication *app, int argc, const char **argv)
				{
					cout << "Starting Station" << argv[0] << endl;
					
					Station* temp = m_cc->incStationPool(atoi(argv[0]));
					
					if(temp == NULL)
					{
						cout << "Station can't be registred, no pool available !" << endl;
						app->Die();
						return;
					}
					
					// Envoie des informations à l'instance
					// Station_1234 id=1 road=1 lines=1,2 len=100
					m_cc->bus->SendMsg("Station_%s id=%d road=%d lines=%s len=%d",argv[0], temp->getID(), temp->getRoadID(), temp->createLineString(), temp->getRoadDistance());
				}
			
			private:
				ControlCenter* m_cc;	
		};
	}

}

#endif //__BUS_STARTMSG__
