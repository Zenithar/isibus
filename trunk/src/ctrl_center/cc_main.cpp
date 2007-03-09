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
#include <pthread.h>

using namespace isibus;

int ControlCenter::mainLoop()
{	
	bus->ivyMainLoop();
	return 0;
}

ControlCenter::ControlCenter()
{
	m_RoadMap = new RoadMap();
	
	bus = new Ivy( "isiBusCC", "isiBusCC READY", this);
	bus->start(NULL);
	
	// Connexion aux services
	bus->BindMsg( "^Bus Start id=(.*)", new msg::BusStartMsg(this) );
	//bus->BindMsg( "^Station Start id=(.*)", new StationStartMsg(this) );
	
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
