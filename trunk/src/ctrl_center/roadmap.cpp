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

vector<string> RoadMap::split(const string &sep,string text)
{
	vector<string> words;
	string::size_type end;
	do
	{
		end = text.find(sep);
		if (end == string::npos)
			end = text.length() + 1;

		words.push_back(text.substr(0,end));
		text.replace(0,end+sep.length(),"");

	} while (text.length());
	return words;
}

string RoadMap :: createLinePath(const int line)
{
	std::vector<isibus::Road*> rl;
	stringstream ret;
	
	rl = m_LineList[line];
	
	ret << line << ":";
	for(int i=0; i<rl.size(); i++)
	{
		ret << rl[i]->getID() << "," << rl[i]->getLen() << ";";
	}
	
	return ret.str();	
}

bool RoadMap::loadMap(const std::string& filename, bool verbose)
{
	XMLNode xMainNode=XMLNode::openFileHelper(filename.c_str(), "isibus");
	
	// Chargement des rues
	XMLNode xCity=xMainNode.getChildNode("city");
	XMLNode xRoads=xCity.getChildNode("roads");
	int speed_limitation = atoi(xRoads.getAttribute("limit"));
	m_iNbRoads = xRoads.nChildNode();
	
	if(verbose) {
		cout << "Roads (limitation " << speed_limitation << " Km/h)" << endl;
		cout << "Processing " << m_iNbRoads << " road segment ..." << endl;
	}
	
	int xmlIterator=0;
	for(int i=0;i<m_iNbRoads;i++)
	{
		XMLNode xRoad = xRoads.getChildNode("road", &xmlIterator);
		
		if(verbose) {
			cout << "[" << xRoad.getAttribute("id") << "] " << xRoad.getAttribute("name");
			cout << " type: " << xRoad.getAttribute("type") << " len: " << xRoad.getAttribute("len") <<"m" << endl;
		}
		
		Road* temp = new Road(atoi(xRoad.getAttribute("id")), xRoad.getAttribute("name"), atoi(xRoad.getAttribute("type")), atoi(xRoad.getAttribute("len")), xRoad.getAttribute("axe")[0]);
		
		m_RoadList.insert(std::make_pair(atoi(xRoad.getAttribute("id")), temp));
	}
	
	// Création du graphe
	XMLNode xGraph=xCity.getChildNode("graph");
	int nb_node = xGraph.nChildNode();
	
	if(verbose)
		cout << endl << "Building the city map with " << nb_node << " nodes." << endl;
	
	xmlIterator=0;
	int node_id = 1;
	for(int i=0;i<nb_node;i++)
	{
		XMLNode xNode = xGraph.getChildNode("node", &xmlIterator);
		
		if(verbose) {
			cout << "Connection [" << node_id << "] to roads (" << xNode.getAttribute("roads") << ")" << endl;
		}
		
		ni_t node = m_RoadGraph.insert(node_id);
		
		vector<string> roads = split(",", xNode.getAttribute("roads"));
		for(int j=0; j<roads.size(); j++)
		{
			if(verbose) {
				cout << " + Connection " << node_id << " to road " << m_RoadList[atoi(roads[j].c_str())]->getName() << endl;
			}
		}
		
		node_id++;
	}
	
	// Création des lignes de bus
	XMLNode xLines=xMainNode.getChildNode("lines");
	m_iNbLines = xLines.nChildNode();
	
	if(verbose) {
		cout << endl << "Populating bus lines with " << m_iNbLines << " lines." << endl;
	}
	
	xmlIterator=0;
	for(int i=0;i<m_iNbLines;i++)
	{		
		std::vector<isibus::Road*> rl;
		XMLNode xLine = xLines.getChildNode("line", &xmlIterator);
		
		if(verbose) {
			cout << "[" << xLine.getAttribute("id") << "] " << xLine.getAttribute("roads") << endl;
		}
		
		vector<string> roads = split(",", xLine.getAttribute("roads"));
		
		for(int j=0; j < roads.size(); j++) 
		{
			rl.push_back(m_RoadList[atoi(roads[j].c_str())]);
		}
		
		m_LineList[atoi(xLine.getAttribute("id"))] = rl;
	}
	
	// Positionnement des stations de bus.
	XMLNode xStations=xMainNode.getChildNode("stations");
	m_iNbStations = xStations.nChildNode();
	
	if(verbose) {
		cout << endl << "Populating bus stations with " << m_iNbStations << " stations." << endl;
	}
	
	xmlIterator=0;
	int station_id = 1;
	for(int i=0;i<m_iNbStations;i++)
	{
		XMLNode xStation = xStations.getChildNode("station", &xmlIterator);
		
		if(verbose) {
			cout << "[" << station_id << "] road:" << xStation.getAttribute("road") << " line:" << xStation.getAttribute("lines") << endl;
		}
		
		Station* temp = new Station(station_id, atoi(xStation.getAttribute("road")));
		vector<string> lines = split(",", xStation.getAttribute("lines"));
		
		m_StationList.insert(std::make_pair(station_id++, temp));
	}

	// Enregistrement des bus.
	XMLNode xTransport=xMainNode.getChildNode("transport");
	m_iNbBus = xTransport.nChildNode();
	
	if(verbose) {
		cout << endl << "Registering " << m_iNbBus << " bus." << endl;
	}
	
	xmlIterator=0;
	for(int i=0;i<m_iNbBus;i++)
	{
		XMLNode xBus = xTransport.getChildNode("bus", &xmlIterator);
		
		if(verbose) {
			cout << "[" << xBus.getAttribute("id") << "] capacity:" << xBus.getAttribute("passengers") << " line:" << createLinePath(atoi(xBus.getAttribute("line"))) << endl;
		}
		
		Bus* temp = new Bus(atoi(xBus.getAttribute("id")), atoi(xBus.getAttribute("passengers")), atoi(xBus.getAttribute("line")));
		
		m_BusList.insert(std::make_pair(atoi(xBus.getAttribute("id")), temp));
	}
	
	return true;
}

} // isibus
