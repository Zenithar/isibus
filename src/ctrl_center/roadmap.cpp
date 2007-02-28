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
#include "xmlParser.h"

using namespace std;

namespace isibus {

RoadMap::RoadMap()
{
}


RoadMap::~RoadMap()
{
}

bool RoadMap::loadMap(const std::string& filename)
{
	XMLNode xMainNode=XMLNode::openFileHelper(filename.c_str(), "isibus");
	
	// Chargement des rues
	XMLNode xCity=xMainNode.getChildNode("city");
	XMLNode xRoads=xCity.getChildNode("roads");
	int speed_limitation = atoi(xRoads.getAttribute("limit"));
	int nb_road = xRoads.nChildNode();
	
	cout << "Roads (limitation " << speed_limitation << " Km/h)" << endl;
	cout << "Processing " << nb_road << " road segment ..." << endl;
	
	int xmlIterator=0;
	for(int i=0;i<nb_road;i++)
	{
		XMLNode xRoad = xRoads.getChildNode("road", &xmlIterator);
		cout << "[" << xRoad.getAttribute("id") << "] " << xRoad.getAttribute("name");
		cout << " type: " << xRoad.getAttribute("type") << " len: " << xRoad.getAttribute("len") <<"m" << endl;
	}
	
	// Création du graphe
	XMLNode xGraph=xCity.getChildNode("graph");
	int nb_node = xGraph.nChildNode();
	
	cout << endl << "Building the city map with " << nb_node << " nodes." << endl;
	
	xmlIterator=0;
	int node_id = 1;
	for(int i=0;i<nb_node;i++)
	{
		XMLNode xNode = xGraph.getChildNode("node", &xmlIterator);
		cout << "Connection [" << node_id++ << "] to roads (" << xNode.getAttribute("roads") << ")" << endl;
	}
	
	// Création des lignes de bus
	XMLNode xLines=xMainNode.getChildNode("lines");
	int nb_lines = xLines.nChildNode();
	
	cout << endl << "Populating bus lines with " << nb_lines << " lines." << endl;
	
	xmlIterator=0;
	for(int i=0;i<nb_lines;i++)
	{
		XMLNode xLine = xLines.getChildNode("line", &xmlIterator);
		cout << "[" << xLine.getAttribute("id") << "] " << xLine.getAttribute("roads") << endl;
	}
	
	// Positionnement des stations de bus.
	XMLNode xStations=xMainNode.getChildNode("stations");
	int nb_stations = xStations.nChildNode();
	
	cout << endl << "Populating bus stations with " << nb_stations << " stations." << endl;
	
	xmlIterator=0;
	int station_id = 1;
	for(int i=0;i<nb_stations;i++)
	{
		XMLNode xStation = xStations.getChildNode("station", &xmlIterator);
		cout << "[" << station_id++ << "] road:" << xStation.getAttribute("road") << " line:" << xStation.getAttribute("lines") << endl;
	}
	
	// Enregistrement des bus.
	XMLNode xTransport=xMainNode.getChildNode("transport");
	int nb_transports = xTransport.nChildNode();
	
	cout << endl << "Registering " << nb_transports << " bus." << endl;
	
	xmlIterator=0;
	for(int i=0;i<nb_transports;i++)
	{
		XMLNode xBus = xTransport.getChildNode("bus", &xmlIterator);
		cout << "[" << xBus.getAttribute("id") << "] capacity:" << xBus.getAttribute("passengers") << " line:" << xBus.getAttribute("line") << endl;
	}
	
	return true;
}
} // isibus
