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
 
#ifndef __STATION_PATHMSG__
#define __STATION_PATHMSG__

#include <Ivycpp.h>
#include <IvyApplication.h>

namespace isibus 
{
	class ControlCenter;
	
	namespace msg 
	{
		class StationPathMsg : public IvyMessageCallback {
			public:
				StationPathMsg(ControlCenter* cc):m_cc(cc) {}
			
				void OnMessage(IvyApplication *app, int argc, const char **argv)
				{
					string s = m_cc->m_RoadMap->StationPath(atoi(argv[0]), atoi(argv[1]));
					m_cc->bus->SendMsg("CC Path=%s", s.c_str());
				}
			
			private:
				ControlCenter* m_cc;	
		};
	}

}

#endif //__STATION_PATHMSG__
