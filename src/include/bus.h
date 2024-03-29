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

#ifndef BUS_H
#define BUS_H

#include <stdlib.h>

#include "Ivycpp.h"
#include "IvyApplication.h"

#include <string>
using namespace std;

namespace isibus
{
/**
	Classe de stockage des informations associées au Bus.
	@author Thibault Normand <thibault.normand@gmail.com>
*/

enum BUS_STATUS {
	RUNNING = 0,
	ONSTATION = 1,
	PBONBOARD = 2,
	OUTOFORDER = 3
};

class Bus
{
public:
        Bus(int id, int capacity, int line, int roadid):m_iBus_id(id), m_iCapacity(capacity), m_iLine_id(line), m_iRoadID(roadid)
	{
		m_iSpeed = 0;
		m_iRoadDistance = 0;
	};

        ~Bus() {
		IvyC::IvySendDieMsg(IvyC::IvyGetApplication( (char*)m_szAppName.c_str() ));
	};

        int getID() const
        {
                return m_iBus_id;
        }

        void setCapacity(int theValue)
        {
                m_iCapacity = theValue;
        }


        int getCapacity() const
        {
                return m_iCapacity;
        }

        void setLine(int theValue)
        {
                m_iLine_id = theValue;
        }


        int getLine() const
        {
                return m_iLine_id;
        }

	void Start()
	{
		system("./lanceur_bus > /dev/null &");
	}

	void setRoadDistance(int theValue)
	{
	        m_iRoadDistance = theValue;
	}
	

	int getRoadDistance() const
	{
	        return m_iRoadDistance;
	}

	void setRoadID(int theValue)
	{
	        m_iRoadID = theValue;
	}
	

	int getRoadID() const
	{
	        return m_iRoadID;
	}

	void setSpeed(int theValue)
	{
	        m_iSpeed = theValue;
	}
	

	int getSpeed() const
	{
	        return m_iSpeed;
	}
	
	void setStatus(int theValue)
	{
		m_iStatus = theValue;
	}
	
	int getStatus() const
	{	
		return m_iStatus;
	}

	void setAppName(const string& theValue)
	{
	        m_szAppName = theValue;
	}
	

	string getAppName() const
	{
	        return m_szAppName;
	}
	
private:
        int m_iBus_id;	
        int m_iLine_id;
        int m_iCapacity;
	int m_iSpeed;
	int m_iRoadID;
	int m_iRoadDistance;
	int m_iStatus;
	
	string m_szAppName;

};

}
#endif
