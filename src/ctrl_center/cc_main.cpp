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

using namespace isibus;

bool ControlCenter::startControl()
{
	m_RoadMap = new RoadMap();
	m_RoadMap->loadMap("roadmap.xml");
}

ControlCenter::~ControlCenter()
{
	delete m_RoadMap;
}
