#include "Isibus.h"
#include "../base/xmlParser.h"
//#include "msgs.h"
#include "../include/msg/ui/ui_msg.h"
#include "../include/msg/ui/ui_move_bus.h"

#define IMG_BACKGROUND "pics/route_herbe.png"

#define BG_W 1024
#define BG_H 1024

using namespace isibus;

static struct
{
	int id;
	const char *path;
}
bus_animations [] =
{
	{ ID_BUS_LEFT_TO_RIGHT, "pics/bus_*.png"},
	{ ID_BUS_RIGHT_TO_LEFT, "pics/bus_rtol.png"},
	{ ID_BUS_FRONT_TO_BACK, "pics/bus_ftob.png"},
	{ ID_BUS_BACK_TO_FRONT, "pics/bus_fbof.png"},
	{ ID_CASE_ROUTE,	"pics/route_*.png"},
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

void Isibus::initIvy()
{
	worker = new IvyWorker(this);
	
	connect(worker, SIGNAL(addMessage(const QString &)),
		 this, SLOT(ajouterMessage(const QString &)));

	connect(worker, SIGNAL(moveBus(const int &, const int &, const int &, const int &, const int &, const int &)),
		 this, SLOT(bougerBus(const int &, const int &, const int &, const int &, const int &, const int &)));
	
	worker->start();
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
	field->setBackgroundBrush( pm );

	readSprites();
	genererCarte(FALSE);
	// début de la simulation
        mTimerId = startTimer( 9 );

	// initialisation du bus
	initIvy();

	// connect pour l'ajout d'un bus
	connect(widget.addBusButton, SIGNAL( pressed() ), SLOT( addBus() ));
	// connect pour la suppression d'un bus
	// connect pour le menu
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
	int X = BG_W/2;
	int Y = BG_H/2;
	int newX;
	int newY;
	int i;
	XMLNode xMainNode=XMLNode::openFileHelper("roadmap.xml", "isibus");
	
	// Chargement des rues
	XMLNode xCity=xMainNode.getChildNode("city");
	XMLNode xRoads=xCity.getChildNode("roads");
	int speed_limitation = atoi(xRoads.getAttribute("limit"));
	int m_iNbRoads = xRoads.nChildNode();
	
	int xmlIterator=0;
	RoadCase* temp;
	for(i=0;i<m_iNbRoads;i++)
	{
		XMLNode xRoad = xRoads.getChildNode("road", &xmlIterator);
		int j = 0;
		for(int i = 0; i <  ((atoi(xRoad.getAttribute("len")) / 50)); i++){
			switch(xRoad.getAttribute("axe")[0])
			{ 	case 'S': Y +=  27; 
					break;
				case 'N': Y -=  27;
 					break;
				case 'E': X +=  33; 
					break;
				default: X -=  33; 
					break;
			}	

		temp = new RoadCase(mAnimation.value(ID_CASE_ROUTE), field, X, Y, xRoad.getAttribute("axe")[0], 0, i,atoi(xRoad.getAttribute("id")));
		if(i ==  ((atoi(xRoad.getAttribute("len")) / 50)-1))
		{
			
			temp ->setFrame(2);
		}
		else{
			if(xRoad.getAttribute("axe")[0] == 'S' || xRoad.getAttribute("axe")[0] == 'N')
			{temp ->setFrame(9);} 
			
			if(xRoad.getAttribute("axe")[0] == 'E' || xRoad.getAttribute("axe")[0] == 'W')
			{temp ->setFrame(7);} 
		}
		j ++;	
		roadcaselist.push_back(temp);
		}
		
		//m_RoadCaseList.insert(std::make_pair(j, temp));
	}


	// Positionnement des stations de bus.

	XMLNode xStations=xMainNode.getChildNode("stations");
	int m_iNbStations = xStations.nChildNode();
	
	
	xmlIterator=0;
	int station_id = 1;
	for(int i=0;i<m_iNbStations;i++)
	{
		XMLNode xStation = xStations.getChildNode("station", &xmlIterator);
		foreach	(RoadCase * rc, roadcaselist){
			int X = rc->x;
			int Y = rc->y;
			if((rc->idRoad == atoi(xStation.getAttribute("road"))) && (rc->segment == (atoi(xStation.getAttribute("len"))/100)))
			{
				rc->idArret = atoi(xStation.getAttribute("id"));
				if((rc->direction == 'E') || (rc->direction == 'W'))
				{
					rc->translate(0.0,-11.0);
					rc->setFrame(0);
				} 
			
				if((rc->direction == 'S') || (rc->direction == 'N'))
				{
					rc->translate(-13.0,0.0);
					rc->setFrame(1);
					
				} 
			}

		}

		
		//Station* temp = new Station(station_id, atoi(xStation.getAttribute("road")));
		//vector<string> lines = split(",", xStation.getAttribute("lines"));
		
		//m_StationList.insert(std::make_pair(station_id++, temp));
	}
/*
	// Enregistrement des bus.
	XMLNode xTransport=xMainNode.getChildNode("transport");
	m_iNbBus = xTransport.nChildNode();
	
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

	foreach	(RoadCase * rc, roadcaselist){
		rc->setPos( rc->x, rc->y );
        	rc->show( );
		}


}

void Isibus::timerEvent( QTimerEvent * )
{
	field->advance();

	foreach (BusSprite* bus ,buses){
		wrapSprite( bus );
	}

}

void Isibus::wrapSprite( IsiSprite *s )
{
	int randNb;
	qreal coefDeplacement = 0.001;
	RoadCase* rc;

	if (field->itemAt(s->x(),s->y())){
		//rc = field->itemAt(s->xVelocity(),s->yVelocity());
		//int y = field->itemAt(s->xVelocity(),s->yVelocity());
		//printf("Le bus est sur la route! Direction de la route\n");// : %c\n", rc->direction);
	}

	/*********************************************************************/
	/*              Déplacement automatique du bus			     */
	/*********************************************************************/
/*
	// Si le bus atteint un bord il est recréé sur le bord opposé
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
*/
/*
	srand(time(NULL));
	// Axe des ordonnées aléatoire
	randNb = rand();
	if (randNb>(2*(RAND_MAX/3))){s->setVelocity(s->xVelocity()+coefDeplacement,s->yVelocity());}
	if (randNb<(RAND_MAX/3)){s->setVelocity(s->xVelocity()-coefDeplacement,s->yVelocity());}

	srand(time(NULL));
	// Axe des abscisses aléatoire
	randNb = rand();
	if (randNb>(2*(RAND_MAX/3))){s->setVelocity(s->xVelocity(),s->yVelocity()+coefDeplacement);}
	if (randNb<(RAND_MAX/3)){s->setVelocity(s->xVelocity(),s->yVelocity()-coefDeplacement);}

	// ajuste dynamiquement l'apparence du bus
	if (s->xVelocity()==0 || s->yVelocity()==0 ) {s->setFrame(1);}
	if (s->xVelocity()<0 || fabs(s->yVelocity()) < fabs(s->xVelocity()) ) {s->setFrame(0);}
	if (s->yVelocity()<0 || fabs(s->xVelocity()) < fabs(s->yVelocity()) ) {s->setFrame(1);}
	if (s->yVelocity()>0 || fabs(s->xVelocity()) < fabs(s->yVelocity()) ) {s->setFrame(2);}
	if (s->xVelocity()>0 || fabs(s->yVelocity()) < fabs(s->xVelocity()) ) {s->setFrame(3);}
*/
}

void Isibus::addBus()
{
        // Création de l'objet graphique
	BusSprite *newBus = new BusSprite( mAnimation.value(ID_BUS_LEFT_TO_RIGHT), field, ID_BUS_LEFT_TO_RIGHT, 2, 1 );
        double dx = 0.0;
        double dy = 0.0;
        newBus->setVelocity( dx, dy );
        newBus->setPos(BG_W/2, BG_H/2 );
	newBus->setZValue(1.0);
	newBus->setFrame(1);
	
	// Envoie du message de création au centre de commande.
	//worker->bus->SendMsg("cc createBus id=%d passengers=%d line=%d", 10,30,1);
worker->bus->SendMsg("");

//Bus_%s id=%d passengers=%d line=%d",argv[0], temp->getID(), temp->getCapacity(), temp->getLine()
	
	buses.push_back(newBus);
	newBus->show( );
}

void Isibus::ajouterMessage(const QString& message)
{
	widget.lw_historique->addItem(message);
}

void Isibus::bougerBus(const int &id, const int &ligne,const int &route,const int &segment,const int &capacite,const int &vitesse)
{
	cout<<"coucou2"<<endl;

}

/**********************************************************************************************
 * IvyWorker
***********************************************************************************************/

IvyWorker::IvyWorker(QObject * parent):QThread(parent)
{
	bus = new Ivy( "isiBusUI", "isiBusUI READY", this);
	bus->start(NULL);
	
bus->BindMsg("^Bus id=([0-9]+) line=([0-9]+) pos=([0-9]+),([-]?[0-9]+) capacity=([0-9]+) speed=([0-9]+)", new msg::uiMvBus( this, qobject_cast<Isibus*>(parent) ));

	bus->BindMsg("(.*)", this);
	
	bus->BindMsg("(.*)", new msg::UiMsg( this, qobject_cast<Isibus*>(parent) ));

;
}

void IvyWorker::run()
{
	IvyC::IvyMainLoop ();
}
		
void IvyWorker :: OnMessage(IvyApplication *app, int argc, const char **argv)
{
	emit addMessage(QString(argv[0]));
}

void IvyWorker :: OnApplicationConnected (IvyApplication *app)
{
	const char *appname;
	const char *host;
	appname = app->GetName();
	host = app->GetHost();

	printf("%s connected from %s\n", appname,  host);

}
void IvyWorker :: OnApplicationDisconnected (IvyApplication *app)
{
	const char *appname;
	const char *host;
	appname = app->GetName ();
	host = app->GetHost();

	printf("%s disconnected from %s\n", appname,  host);
}

void IvyWorker :: OnDirectMessage (IvyApplication *app, int id, const char *arg )
{
	printf ("%s direct sent %d %s",app->GetName(), id, arg);
}

#include "Isibus.moc"
