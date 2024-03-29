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

#ifndef ISIBUSROADMAP_H
#define ISIBUSROADMAP_H

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

#include <graph-c++.h>
#include <graph-c++-comp.h>

#include <road.h>
#include <station.h>
#include <bus.h>

using namespace graph;

typedef Graph<int, isibus::Road*, false> RoadGraph;
typedef Graph<int, isibus::Road*, false>::node_iterator ni_t;
typedef Graph<int, isibus::Road*, false>::edge_iterator ei_t;

typedef std::map<int, isibus::Road*> RoadList;
typedef std::map<int, isibus::Station*> StationList;
typedef std::map<int, isibus::Bus*> BusList;

typedef std::map<int, std::vector<isibus::Road*> > LinesList;

namespace isibus {

/**
Classe modélisant le réseaux routier
@author Thibault Normand <thibault.normand@gmail.com>
*/
class RoadMap{
public:
	RoadMap();
	~RoadMap();
    
	bool loadMap(const std::string& filename, bool verbose = false);
	
private:
	vector<string> split(const string &sep,string text);
	vector<Road*> dijkstra(int src, int dest);
	
	int		m_iNbRoads;
	int		m_iNbBus;
	int		m_iNbStations;
	int		m_iNbLines;
	
	RoadList	m_RoadList;
	StationList	m_StationList;
	BusList		m_BusList;
	LinesList	m_LineList;
	
	RoadGraph	m_RoadGraph;

public:
	string createLinePath(const int line);
	string StationPath(int s_id, int d_id);
			
	inline int getNbRoads() { return m_iNbRoads; }
	inline int getNbBus() { return m_iNbBus; }
	inline int getNbStations() { return m_iNbStations; }
	inline int getNbLines() { return m_iNbLines; }	
	
	inline RoadList& getRoadList() { return m_RoadList; }
	inline BusList& getBusList() { return m_BusList; }
	inline StationList& getStationList() { return m_StationList; }
	inline LinesList& getLineList() { return m_LineList; }
};

}

#endif
