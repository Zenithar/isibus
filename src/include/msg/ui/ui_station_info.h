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
 
#ifndef __UI_INFO_STATION__
#define __UI_INFO_STATION__

#include <Ivycpp.h>
#include <IvyApplication.h>

namespace isibus 
{
	class Isibus;
	class IvyWorker;
	
	namespace msg 
	{
		class uiInfoStation : public IvyMessageCallback {
		private:
			//QListWidget * qlw;
			Isibus * isb;
			IvyWorker* ivy;
		public:

			//UiMsg(QListWidget * _qlw): qlw(_qlw) {}
			uiInfoStation(IvyWorker* _ivy, Isibus * _isb):ivy(_ivy), isb(_isb) {}
			
			QString ChangeStatus(int i)
			{
				switch(i){
					case 0: return "En route";
					case 1: return "A un arret";
					case 2: ;
					case 3: return "Problemes";
					default: return "Plus de bus";
				}
			}
			
			void OnMessage(IvyApplication *app, int argc, const char **argv)
			{
				int t = argc;
				int i = 1;

				int id;
				int id1 = 0;
				int id2 = 0;
				int id3 = 0;
				int h1 = 0;
				int h2 = 0;
				int h3 = 0;
				QString s1 = "Plus de Bus";
				QString s2 ="Plus de Bus";
				QString s3 ="Plus de bus";
				cout<<"coucou"<<endl;
				if(argc == 4)
				{ 
				 		id = atoi(argv[0]);
				 		id1 = atoi(argv[1]);
				 		h1 = atoi(argv[2]);
				 		s1 = ChangeStatus(atoi(argv[3]));
				 		id2 = 0;
				 		h2 = 0;
				 		s2 = ChangeStatus(9);
				 		id3 = 0;
				 		h3 = 0;
						s3 = ChangeStatus(9);
				}else
					{
						if(argc == 7)
						{
				 		id = atoi(argv[0]);
				 		id1 = atoi(argv[1]);
				 		h1 = atoi(argv[2]);
				 		s1 = ChangeStatus(atoi(argv[3]));
				 		id2 = atoi(argv[4]);
				 		h2 = atoi(argv[5]);
				 		s2 = ChangeStatus(atoi(argv[6]));
				 		id3 = 0;
				 		h3 = 0;
						s3 = ChangeStatus(9);
						}else{ if(argc >= 10)
							{   
				 		id = atoi(argv[0]);
				 		id1 = atoi(argv[1]);
				 		h1 = atoi(argv[2]);
				 		s1 = ChangeStatus(atoi(argv[3]));
				 		id2 = atoi(argv[4]);
				 		h2 = atoi(argv[5]);
				 		s2 = ChangeStatus(atoi(argv[6]));
				 		id3 = atoi(argv[7]);
				 		h3 = atoi(argv[8]);
						s3 = ChangeStatus(atoi(argv[9]));
							}
						}
					}
					ivy->RefreshStationInfo(id,id1,h1,s1,id2,h2,s2,id3,h3,s3);			
				}
		};
	}

}
//Station id= 1 time= 1 status= 0
#endif //__BUS_STARTMSG__
