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

#include "roadmap.h"

namespace isibus {

class ControlCenter
{
	private:
		bool	m_bRunning;

	public:
		ControlCenter():m_RoadMap(NULL) {}
		~ControlCenter();
		
	/****************************************************************/
	/* Accesseurs 							*/
	/****************************************************************/

		/** 
		 * Retourne l'etat d'execution du centre de controle.
		*/
		inline bool isRunning() { return m_bRunning; }
		inline bool stop() { m_bRunning = false; }

		/**
		 * Démarre le centre de contrôle.
		 * @return etat de l'execution.
		*/
		bool	startControl();
		
	private:
		/**
		 * Accès au reseau urbain.
		*/
		RoadMap*	m_RoadMap;
};

} // isibus
