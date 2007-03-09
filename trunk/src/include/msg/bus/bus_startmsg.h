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
 
#ifndef __BUS_STARTMSG__
#define __BUS_STARTMSG__

#include <Ivycpp.h>
#include <IvyApplication.h>

namespace isibus 
{
	class ControlCenter;
	
	namespace msg 
	{
		class BusStartMsg : public IvyMessageCallback {
		public:
			BusStartMsg(ControlCenter* cc):m_cc(cc) {}
			
			void OnMessage(IvyApplication *app, int argc, const char **argv)
			{
				cout << "Starting Bus" << argv[0] << endl;
			}
			
		private:
			ControlCenter* m_cc;	
		};
	}

}

#endif //__BUS_STARTMSG__
