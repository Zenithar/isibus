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

void* run(void* arg)
{
	ControlCenter* cc = (ControlCenter*)arg;
	
	cc->bus->ivyMainLoop();	
}

bool ControlCenter::startControl()
{	
	bus = new Ivy( "isiBusCC", "isiBusCC READY", this);
	bus->BindMsg( "^Bus id=(.*) pos=(.*),(.*)", this );
	bus->BindDirectMsg(this);
	
	bus->start(NULL);
	//thread1 = pthread_create( &thread1, NULL, &run, (void*)this);
}

ControlCenter::ControlCenter()
{
	m_RoadMap = new RoadMap();
}


ControlCenter::~ControlCenter()
{
	delete bus;
	delete m_RoadMap;
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
