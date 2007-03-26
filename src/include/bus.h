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

namespace isibus
{
/**
	Classe de stockage des informations associées au Bus.
	@author Thibault Normand <thibault.normand@gmail.com>
*/
class Bus
{
public:
        Bus(int id, int capacity, int line):m_iBus_id(id), m_iCapacity(capacity), m_iLine_id(line)
	{
	};

        ~Bus();

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
		system("bus_de_ligne &");
	}
private:
        int m_iBus_id;
        int m_iLine_id;
        int m_iCapacity;
};

}
#endif
