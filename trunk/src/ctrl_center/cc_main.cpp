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

#include "cc.h"
#include "msgs.h"

#include <pthread.h>

using namespace isibus;

int ControlCenter::mainLoop()
{	
	bus->ivyMainLoop();
	return 0;
}

ControlCenter::ControlCenter()
{
	m_nbRunningBus = 0;
	BusPool.clear();
	
	m_RoadMap = new RoadMap();
	
	bus = new Ivy( "isiBusCC", "isiBusCC READY", this);
	bus->start(NULL);
	
	// Connexion aux services
	bus->BindMsg( "^Bus_([0-9]+) Start", new msg::BusStartMsg(this) );
	bus->BindMsg( "^Bus id= ([0-9]+) line= ([0-9]+) pos= ([0-9]+), ([-]?[0-9]+) capacity= ([0-9]+) speed= ([0-9]+) status= ([0-9]+)", new msg::BusPositionMsg(this) );
	
	bus->BindMsg( "^Station_([0-9]+) Start", new msg::StationStartMsg(this) );
	
	bus->BindMsg( "^gui createBus passengers=([0-9]+) line=([0-9]+)", new msg::BusCreateMsg(this));
	
	bus->BindDirectMsg(this);
}


ControlCenter::~ControlCenter()
{
	delete bus;
	delete m_RoadMap;
}

void ControlCenter :: loadMap(const std::string& filename, bool verbose)
{
	m_RoadMap->loadMap(filename, verbose);
}

const char* ControlCenter :: createLinePath(const int line)
{
	return m_RoadMap->createLinePath(line).c_str();
}

Bus* ControlCenter :: incBusPool(int app_id)
{
	Bus* temp = NULL;
	BusList::iterator iter = m_RoadMap->getBusList().begin();
	int i = 0;
	for( iter; iter != m_RoadMap->getBusList().end();) {
		if(i >= getNbRunningBus()) {
			cout << "Bus_" << app_id << " affected to bus id=" << iter->second->getID() << endl; 
			
			BusPool[app_id] = iter->second->getID();
			m_nbRunningBus++;
			return iter->second;
		}
		i++; iter++;
	}
}

void ControlCenter :: decBusPool(int app_id)
{
	bus->SendMsg("Bus_%d Stop", app_id);
}

Station* ControlCenter :: incStationPool(int app_id)
{
	Station* temp;
	StationList::iterator iter = m_RoadMap->getStationList().begin();
	int i = 0;
	for( iter; iter != m_RoadMap->getStationList().end();) {
		if(i >= getNbRunningBus()) {
			cout << "Station_" << app_id << " affected to Station id=" << iter->second->getID() << endl; 
			
			StationPool[app_id] = iter->second->getID();
			m_nbRunningStation++;
			return iter->second;
		}
		i++; iter++;
	}
}

void ControlCenter :: decStationPool(int app_id)
{
	bus->SendMsg("Station_%d Stop", app_id);
}

Bus* ControlCenter :: registerBus(int capacity, int line)
{
	Bus* b = NULL;
	
	b = new Bus(getNbRunningBus()+1, capacity, line, m_RoadMap->getLineList()[line][0]->getID());
		
	m_RoadMap->getBusList().insert(std::make_pair(b->getID(), b));
	b->Start();
	
	return b;
}

void ControlCenter :: OnMessage(IvyApplication *app, int argc, const char **argv)
{
	int i;
	printf ("%s sent ",app->GetName());
	for  (i = 0; i < argc; i++)
		printf(" '%s'",argv[i]);
	printf("\n");
}

void ControlCenter :: OnApplicationConnected (IvyApplication *app)
{
	const char *appname;
	const char *host;
	appname = app->GetName();
	host = app->GetHost();

	printf("%s connected from %s\n", appname,  host);

}
void ControlCenter :: OnApplicationDisconnected (IvyApplication *app)
{
	const char *appname;
	const char *host;
	appname = app->GetName ();
	host = app->GetHost();

	printf("%s disconnected from %s\n", appname,  host);
}

void ControlCenter :: OnDirectMessage (IvyApplication *app, int id, const char *arg )
{
	printf ("%s direct sent %d %s",app->GetName(), id, arg);
}
