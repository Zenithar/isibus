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
 
#ifndef __UI_MOVE_BUS__
#define __UI_MOVE_BUS__

#include <Ivycpp.h>
#include <IvyApplication.h>
//#include <QListWidget>


namespace isibus 
{
	class Isibus;
	class IvyWorker;
	
	namespace msg 
	{
		class uiMvBus : public IvyMessageCallback {
		private:
			//QListWidget * qlw;
			Isibus * isb;
			IvyWorker* ivy;
		public:
		signals:
		void moveBus(const int &id, const int &ligne,const int &route,const int &segment,const int &capacite,const int &vitesse);

		public:
			//UiMsg(QListWidget * _qlw): qlw(_qlw) {}
			uiMvBus(IvyWorker* _ivy, Isibus * _isb):ivy(_ivy), isb(_isb) {}
			
			void OnMessage(IvyApplication *app, int argc, const char **argv)
			{
				cout<<"coucou"<<endl;
				emit moveBus(1,1,1,1,1,1);
			}

			

		};
	}

}

#endif //__BUS_STARTMSG__
