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

#ifndef __CC_H__
#define __CC_H__

#include "roadmap.h"

#include "Ivycpp.h"
#include "IvyApplication.h"

#include <pthread.h>

namespace isibus
{

/**
	Classe modélisant le centre de contrôle.
	@author Thibault Normand <thibault.normand@gmail.com>
 */
	class ControlCenter : public IvyApplicationCallback, public IvyMessageCallback, public IvyDirectMessageCallback
{
private:
        bool	m_bRunning;

public:
        ControlCenter();

        ~ControlCenter();

        /****************************************************************/
        /* Accesseurs 							*/
        /****************************************************************/

        /**
         * Retourne l'etat d'execution du centre de controle.
        */
        inline bool isRunning()
        {
                return m_bRunning;
        }
        inline bool stop()
        {
                m_bRunning = false;
        }

        /**
         * Démarre le centre de contrôle.
         * @return etat de l'execution.
        */
        int	mainLoop();
	
	/**
	 * Charge la map et les informations associées au reseau.
	*/
	void	loadMap(const std::string& filename, bool verbose);
	
	/**
	 * Retourne une chaine décrivant la ligne du bus.
	*/
	const char* createLinePath(const int line);
	
	/**
	 * Bus logiciel pour la communication inter-processus.
	*/
	Ivy *bus;
	
	/**
	 * Accès au reseau urbain.
	*/
	RoadMap*	m_RoadMap;
	
	void OnApplicationConnected(IvyApplication *app);
	void OnApplicationDisconnected(IvyApplication *app);
	void OnMessage(IvyApplication *app, int argc, const char **argv);
	void OnDirectMessage (IvyApplication *app, int id, const char *arg );
	
	
	inline int getNbRunningBus() { return m_nbRunningBus; }
	Bus* incBusPool(int app_id); 
	void decBusPool(int app_id);
	
	inline int getNbRunningStation() { return m_nbRunningStation; }
	Station* incStationPool(int app_id); 
	void decStationPool(int app_id);
	
	Bus* registerBus(int capacity, int line);
	void BusDie(int bus_id);
	
private:
	/**
	 * Thread d'execution de centre de controle
	*/
	pthread_t thread1;
	
	/**
	 * Nombre de bus en fonctionnement
	*/
	int	m_nbRunningBus;
	std::map<int,int> BusPool;
	
	int	m_nbRunningStation;
	std::map<int,int> StationPool;
	
	fstream filestr;
};

} // isibus

#endif 
