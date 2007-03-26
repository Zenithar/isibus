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
 
#ifndef __UI_MSG__
#define __UI_MSG__

#include <Ivycpp.h>
#include <IvyApplication.h>
//#include <QListWidget>


namespace isibus 
{
		class Isibus;
	namespace msg 
	{
		class UiMsg : public IvyMessageCallback {
		private:
			//QListWidget * qlw;
			Isibus * isb;

		public:
			//UiMsg(QListWidget * _qlw): qlw(_qlw) {}
			UiMsg(Isibus * _isb):isb(_isb) {}
			
			void OnMessage(IvyApplication *app, int argc, const char **argv)
			{				
				isb->ajouterMessage(argv[0]);
			}
			

		};
	}

}

#endif //__BUS_STARTMSG__
