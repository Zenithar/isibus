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
	
vector<string> split(const string &sep,string text)
{
	vector<string> words;
	string::size_type end;
	do
	{
		end = text.find(sep);
		if (end == string::npos)
			end = text.length() + 1;

		words.push_back(text.substr(0,end));
		text.replace(0,end+sep.length(),"");

	} while (text.length());
	return words;
}
			QString ChangeStatus(int i)
			{
				switch(i){
					case 0: return "Tout est OK";
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
				QString s1;
				QString s2 ;
				QString s3;
				cout<<"la taille est de : "<< argc <<" la valeur 1 est "<< argv[0] <<"et le 1:"<< argv[1]<<endl;
				
				id1 = 0;
				h1 = 0;
				s1 = ChangeStatus(9);				
				id2 = 0;
				h2 = 0;
				s2 = ChangeStatus(9);
				id3 = 0;
				h3 = 0;
				s3 = ChangeStatus(9);
				
				QString qs = QString(argv[1]);
				QStringList ql2;
				QStringList ql = qs.split(";");
				id = atoi(argv[0]);
				for(int i = 0; i< ql.size(); i ++)
				{
					ql2 = ql[i].split(", ");
					if( i == 0)
					{
						id1 = atoi(ql2[0]);
						h1 = atoi(ql2[1]);
						s1 = ChangeStatus(atoi(ql2[2]));
					}
					if( i == 2)
					{
						id2 = atoi(ql2[0]);
						h2 = atoi(ql2[1]);
						s2 = ChangeStatus(atoi(ql2[2]));
					}
					if( i == 3)
					{
						id3 = atoi(ql2[0]);
						h3 = atoi(ql2[1]);
						s3 = ChangeStatus(atoi(ql2[2]));
					}

				} 
			
					cout<<id<<";"<<id1<<";"<<h1<<";"<<";"<<id2<<";"<<h2<<";"<<";"<<id3<<";"<<h3<<";"<<endl;
					ivy->RefreshStationInfo(id,id1,h1,s1,id2,h2,s2,id3,h3,s3);			
				}
		};
	}

}
//Station id= 1 time= 1 status= 0
#endif //__BUS_STARTMSG__
