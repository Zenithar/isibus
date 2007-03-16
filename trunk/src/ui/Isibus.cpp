#include "Isibus.h"
#include "../base/xmlParser.h"

#define IMG_BACKGROUND "pics/plain_case.png"

#define BG_W 575
#define BG_H 431


static struct
{
	int id;
	const char *path;
}
bus_animations [] =
{
	{ ID_BUS_LEFT_TO_RIGHT, "pics/bus_1*.png"},
	{ ID_BUS_RIGHT_TO_LEFT, "pics/bus_rtol.png"},
	{ ID_BUS_FRONT_TO_BACK, "pics/bus_ftob.png"},
	{ ID_BUS_BACK_TO_FRONT, "pics/bus_fbof.png"},
	{ ID_CASE_PLAIN,	"pics/plain_case.png"},
	{ 0,                   0 }
};

void Isibus::readSprites()
{
    int i = 0;
    while( bus_animations[i].id )
    {
        QFileInfo fi(bus_animations[i].path );
        QList<QPixmap> *frames = new QList<QPixmap>;
        foreach( const QString &entry, QDir( fi.path(), fi.fileName() ).entryList() )
            frames->append( QPixmap( fi.path() + '/' + entry ) );
        mAnimation.insert( bus_animations[i].id, frames );
        i++;
    }

}


Isibus::Isibus(QWidget *parent) : QMainWindow(parent)
{
	widget.setupUi(this);

	// Creation de la scene ou seront animé les bus
	field = new QGraphicsScene( 0, 0, BG_W, BG_H );
	// Indexation de la scene afin de faciliter la récupération d'items
	field->setItemIndexMethod( QGraphicsScene::NoIndex );

	// On associe la scene à la vue qui va la contenir
	widget.mainView->setScene(field);

	// On transforme le bg en QPixmal afin de pouvoir le manipuler
	QPixmap pm( IMG_BACKGROUND );
	// On associe le nouveau bg à la scene
	//field->setBackgroundBrush( pm );

	readSprites();
	genererCarte(FALSE);
	// début de la simulation
        mTimerId = startTimer( 9 );

	// connect pour l'ajout d'un bus
	connect(widget.addBusButton, SIGNAL( pressed() ), SLOT( addBus() ));
	// connect pour la suppression d'un bus
	// connect pour le menu
}

void Isibus::timerEvent( QTimerEvent * )
{
    field->advance();

    QLinkedList<Bus*>::iterator it;
    for ( it = buses.begin(); it != buses.end(); ++it ) {
        (*it)->nextFrame();
        wrapSprite( *it );
    }

}
/*
vector<string> Isibus::split(const string &sep,string text)
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
}*/

void Isibus::genererCarte(bool verbose)
{
	int X = 100;
	int Y = 100;
	XMLNode xMainNode=XMLNode::openFileHelper("roadmap.xml", "isibus");
	
	// Chargement des rues
	XMLNode xCity=xMainNode.getChildNode("city");
	XMLNode xRoads=xCity.getChildNode("roads");
	int speed_limitation = atoi(xRoads.getAttribute("limit"));
	int m_iNbRoads = xRoads.nChildNode();
	

	
	int xmlIterator=0;
	for(int i=0;i<m_iNbRoads;i++)
	{
		XMLNode xRoad = xRoads.getChildNode("road", &xmlIterator);
		
		for(int i = 0; i <  (atoi(xRoad.getAttribute("len")) / 100); i++){
			switch(xRoad.getAttribute("axe")[0])
			{ 	case 'S': Y -=  27; break;
				case 'N': Y +=  27; break;
				case 'E': X +=  27; break;
				default: X -=  27; break;
			}	
		RoadCase* temp = new RoadCase(mAnimation.value(ID_CASE_PLAIN), field, X, Y, 0, 0, i);
		temp->setPos( X, Y );

		temp->show( );
		}
		
		//m_RoadList.insert(std::make_pair(atoi(xRoad.getAttribute("id")), temp));
	}
	/*
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
		XMLNode xLine = xLines.getChildNode("line", &xmlIterator);
		
		if(verbose) {
			cout << "[" << xLine.getAttribute("id") << "] " << xLine.getAttribute("roads") << endl;
		}
		
		vector<string> roads = split(",", xLine.getAttribute("roads"));
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
			cout << "[" << xBus.getAttribute("id") << "] capacity:" << xBus.getAttribute("passengers") << " line:" << xBus.getAttribute("line") << endl;
		}
		
		Bus* temp = new Bus(atoi(xBus.getAttribute("id")), atoi(xBus.getAttribute("passengers")), atoi(xBus.getAttribute("line")));
		m_BusList.insert(std::make_pair(atoi(xBus.getAttribute("id")), temp));
	}
	*/



}


void Isibus::wrapSprite( IsiSprite *s )
{
    int x = int(s->x() + s->boundingRect().width() / 2);
    int y = int(s->y() + s->boundingRect().height() / 2);

    if ( x > field->width() )
        s->setPos( s->x() - field->width(), s->y() );
    else if ( x < 0 )
        s->setPos( field->width() + s->x(), s->y() );

    if ( y > field->height() )
        s->setPos( s->x(), s->y() - field->height() );
    else if ( y < 0 )
        s->setPos( s->x(), field->height() + s->y() );
}

void Isibus::addBus()
{
        Bus *newBus = new Bus( mAnimation.value(ID_BUS_LEFT_TO_RIGHT), field, ID_BUS_LEFT_TO_RIGHT, 2, 1 );
        double dx = 2.0;
        double dy = 0.0;
        newBus->setVelocity( dx, dy );
        newBus->setPos( 300, 90 );

	newBus->show( );
}
#include "Isibus.moc"
