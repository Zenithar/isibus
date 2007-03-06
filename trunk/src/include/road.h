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

#ifndef ISIBUSROAD_H
#define ISIBUSROAD_H

#include <string>
using namespace std;

namespace isibus
{

/**
	Classe de stockage des informations d'une portion de route.
	@author Thibault Normand <thibault.normand@gmail.com>
*/
class Road
{
private:
        int		m_id;
        string		m_name;
        int		m_type;
        int		m_len;
        char		m_axe;

public:
        Road(int id, const char* name, int type, int len, char axe):m_id(id),m_name(name),m_type(type),m_len(len),m_axe(axe)
        {};

        ~Road();

        void setAxe(char theValue)
        {
                m_axe = theValue;
        }


        char getAxe() const
        {
                return m_axe;
        }

        int getID() const
        {
                return m_id;
        }

        void setLen(int theValue)
        {
                m_len = theValue;
        }


        int getLen() const
        {
                return m_len;
        }

        void setName(const string& theValue)
        {
                m_name = theValue;
        }


        string getName() const
        {
                return m_name;
        }

        void setType(int theValue)
        {
                m_type = theValue;
        }


        int getType() const
        {
                return m_type;
        }

};

}

#endif
