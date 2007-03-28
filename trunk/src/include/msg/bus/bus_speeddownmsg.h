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
 
#ifndef __BUS_SPEEDDOWNMSG__
#define __BUS_SPEEDDOWNMSG__

#include <Ivycpp.h>
#include <IvyApplication.h>

namespace isibus 
{
	class ControlCenter;
	
	namespace msg 
	{
		class BusSpeedDownMsg : public IvyMessageCallback {
			public:
				BusSpeedDownMsg(ControlCenter* cc):m_cc(cc) {}
			
				void OnMessage(IvyApplication *app, int argc, const char **argv)
				{
					m_cc->bus->SendMsg("Bus id=%s speed=-1", argv[0]);
					
					pthread_mutex_lock(&verrou);
					
					Bus* tmp = m_cc->m_RoadMap->getBusList()[atoi(argv[0])];
					tmp->setSpeed(tmp->getSpeed() - 1);
					
					pthread_mutex_unlock(&verrou);
				}
			
			private:
				ControlCenter* m_cc;
				pthread_mutex_t verrou;
		};
	}

}

#endif //__BUS_SPEEDDOWNMSG__
