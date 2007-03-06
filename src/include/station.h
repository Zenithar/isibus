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
        Station(int id, int road_id):m_iStation_id(id), m_iRoad_id(road_id) {};

        ~Station();

        int getID() const
        {
                return m_iStation_id;
        }


private:
        int m_iStation_id;
	int m_iRoad_id;
};

}

#endif
