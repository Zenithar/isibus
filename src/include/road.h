//
// C++ Interface: road
//
// Description: 
//
//
// Author: Thibault Normand <thibault.normand@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ISIBUSROAD_H
#define ISIBUSROAD_H

#include <string>
using namespace std;

namespace isibus {

/**
	@author Thibault Normand <thibault.normand@gmail.com>
*/
class Road{
private:
	int		m_id;
	string		m_name;
	int		m_type;
	int		m_len;
	char		m_axe;
	
public:
    Road(int id, const char* name, int type, int len, char axe):m_id(id),m_name(name),m_type(type),m_len(len),m_axe(axe) {};

    ~Road();

};

}

#endif
