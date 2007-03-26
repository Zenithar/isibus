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
 
#ifndef ISIBUSSTATION_H
#define ISIBUSSTATION_H

namespace isibus
{

/**
	Classe de stockage des informations d'un arrêt.
	@author Thibault Normand <thibault.normand@gmail.com>
*/
class Station
{
public:
        Station(int id, int road_id, int len):m_iStation_id(id), m_iRoad_id(road_id), m_iRoadDistance(len) {};

        ~Station();

        int getID() const
        {
                return m_iStation_id;
        }

	void setRoadID(int theValue)
	{
	        m_iRoad_id = theValue;
	}
	

	int getRoadID() const
	{
	        return m_iRoad_id;
	}

	void setRoadDistance(int theValue)
	{
	        m_iRoadDistance = theValue;
	}
	

	int getRoadDistance() const
	{
	        return m_iRoadDistance;
	}
	
	inline std::vector<int>& getLines() { return m_lLines; }
	
	const char* createLineString() 
	{
		stringstream ret;
		
		ret << m_lLines[0];
		
		for(int i=1; i < m_lLines.size(); i++)
		{
			ret << "," << m_lLines[i];
		}
		
		return ret.str().c_str();
	}

private:
        int m_iStation_id;
	int m_iRoad_id;
	int m_iRoadDistance;
	
	std::vector<int> m_lLines;
};

}

#endif
