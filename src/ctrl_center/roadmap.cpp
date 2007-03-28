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
	cout << "Destruction du reseau routier" << endl;
	
	for(BusList::iterator it = m_BusList.begin();it != m_BusList.end(); it++)
	{
		delete it->second;
	}
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

/**
 * Retourne une chaine de caractère décrivant la route pour une ligne de bus.
*/
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

/**
 * Charge le plan routier depuis un fichier XML
*/
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
	
	// Lecture des croisements
	XMLNode xGraph=xCity.getChildNode("graph");
	int nb_node = xGraph.nChildNode();
	
	if(verbose)
		cout << endl << "Building the city map with " << nb_node << " nodes." << endl;
	
	xmlIterator=0;
	int node_id = 1;
	for(int i=0;i<nb_node;i++)
	{
		XMLNode xNode = xGraph.getChildNode("node", &xmlIterator);
			
		ni_t node = m_RoadGraph.insert(node_id);
		
		vector<string> roads = split(",", xNode.getAttribute("roads"));
		for(int j=0; j<roads.size(); j++)
		{
			m_RoadList[atoi(roads[j].c_str())]->getNodes().push_back(node_id);
		}
		
		node_id++;
	}
	
	// Création du graphe
	for(RoadList::iterator it=m_RoadList.begin(); it != m_RoadList.end(); it++)
	{
		vector<int> tab = (it->second)->getNodes();
		
		if(tab.size() > 2)
		{
			cout << "Roadmap Error ! Duplicate node declaration.";
			return false;
		}
		
		ni_t n1 = m_RoadGraph.find(tab[0]);
		ni_t n2 = m_RoadGraph.find(tab[1]);
		
		n1->insert_edge(n2, (it->second));
		n2->insert_edge(n1, (it->second));
	}
	
	for (ni_t ni = m_RoadGraph.begin(); ni != m_RoadGraph.end(); ni++)
	{
		cout << "Node " << *ni << std::endl;
		for (ei_t ei = ni->begin(); ei != ni->end(); ei++)
			cout << "\t[" << -ei << "-->" << +ei << "] = " << ((Road*)(*ei))->getName() << std::endl;
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
		
		Station* temp = new Station(station_id, atoi(xStation.getAttribute("road")), atoi(xStation.getAttribute("len")));
		
		vector<string> lines = split(",", xStation.getAttribute("lines"));
		for(int j=0; j < lines.size(); j++) 
		{
			temp->getLines().push_back(atoi(lines[j].c_str()));
		}
		
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
		
		Bus* temp = new Bus(atoi(xBus.getAttribute("id")), atoi(xBus.getAttribute("passengers")), atoi(xBus.getAttribute("line")), m_LineList[atoi(xBus.getAttribute("line"))][0]->getID());
		
		m_BusList.insert(std::make_pair(atoi(xBus.getAttribute("id")), temp));
	}
	
	return true;
}

/**
 * Calcul un chemin alternatif entre deux stations en utilisant l'algorithme de Dijkstra. (Shortest Path)
*/
string RoadMap :: StationPath(int s_id, int d_id)
{
	stringstream s;
	
	int road_s = m_StationList[s_id]->getRoadID();
	int road_d = m_StationList[d_id]->getRoadID();
	
	vector<Road*> ret = dijkstra(m_RoadList[road_s]->getNodes()[0], m_RoadList[road_d]->getNodes()[0]);
	
	s << s_id << "-" << d_id << ":";
	
	for(int i=ret.size()-1; i>=0; i--)
	{
		s << ret[i]->getID() << "," << ret[i]->getLen() << ";";
	}
	
	s << m_RoadList[road_d]->getID() << "," << m_RoadList[road_d]->getLen() << ";";
	
	return s.str();
}

vector<Road*> RoadMap :: dijkstra(int src, int dest)
{    
	vector<Road*> ret;
	
	int i;
	
	int n = m_RoadGraph.count_nodes();
    
	/* the distance array */
	int *d = new int[n];
	/* the parent array */
	int *t = new int[n];
	/* the selected array */
	int *s = new int[n];
    
	/* assume weights << 30000 */
	const int INF = 30000;
    
	/* initialize distance, parent and selected arrays */
	for (i = 0; i < n; i++)
	{
		d[i] = INF;
		t[i] = -1;
		s[i] = 0;
	}
    
	/* make sure src will be the first selected node */
	d[src] = 0;
     
	/* while dest not selected */
	while (1)
	{
		int imin = -1;
		int dmin = INF;
	
		/* find the closest node to the selected set */
		for (i = 0; i < n; i++)
			if (!s[i] && 
						  (imin == -1 || d[i] < dmin))
		{
			imin = i;
			dmin = d[i];
		}
	
		/* no such node found, there may be no path to it */
		if (imin < 0)
			break;
	    
		/* select node */
		s[imin] = 1;
	
		/* find its iterator */
		ni_t ni = m_RoadGraph.find(imin);
	
		/* browse through its edges */
		for (ei_t ei = ni->begin(); ei != ni->end(); ei++)
		{
			/* relax its edges */
			int dd = ((Road*)*ei)->getLen()+d[imin];
			if (dd < d[*ei[1]])
			{
				d[*ei[1]] = dd;
				t[*ei[1]] = imin;
			}
		}
	
		/* if dest reached, stop */
		if (imin == dest)
			break;
	}
    
	if (s[dest] == 0)
	{
		std::cout << "Could not reach destination node " << dest << std::endl;
		return ret;
	}
    
	/* output the path (in reverse order) */
	i = dest;
	while (1)
	{
		int j = i;
	
		/* t[i] is i's father */
		i = t[i];
		if (i < 0)
			break;
	    
		/* add the edge cost */
		ret.push_back(((Road*)*(m_RoadGraph.find(i)->find_edge(m_RoadGraph.find(j)))));
	}

	
	return ret;
}

} // isibus
