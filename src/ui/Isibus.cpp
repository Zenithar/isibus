
#include "Isibus.h"



#include "../base/xmlParser.h"
//#include "msgs.h"
#include "../include/msg/ui/ui_msg.h"
#include "../include/msg/ui/ui_move_bus.h"
#include "../include/msg/ui/ui_station_info.h"

#define IMG_BACKGROUND "pics/route_herbe.png"

#define BG_W 1024
#define BG_H 1024
#define RTE_H 27
#define RTE_W 33

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
	
	connect(worker, SIGNAL(sigAddMessage(const QString &)),
		 this, SLOT(ajouterMessage(const QString &)));

	connect(worker, SIGNAL(sigRefreshStationInfo(const int &,const int &,const int &,const QString &,const int &,const int &,const QString &,const int &,const int &,const QString & )),
		 this, SLOT(rafraichirArretInfo(const int &,const int &,const int &,const QString &,const int &,const int &,const QString &,const int &,const int &,const QString & )));

	connect(worker, SIGNAL(sigMoveBus(const int &, const int &, const int &, const int &, const int &, const int &)),
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
        mTimerId = startTimer( 1000 );

	// initialisation du bus
	initIvy();

	iDBusSelected = -1;
	iDarretSelected = -1;
	// Général
	connect(widget.action_Quit, SIGNAL( triggered() ), SLOT( close() ));

	// Propriétés du bus
	// connect pour l'ajout d'un bus
	connect(widget.addBusButton, SIGNAL( pressed() ), SLOT( addBus() ));
	// connect pour la suppression d'un bus
	connect(widget.delBusButton, SIGNAL( pressed() ), SLOT( delBus() ));
	// connect pour ralentir le bus 
	connect(widget.slowBusButton, SIGNAL( pressed() ), SLOT( slowBus() ));
	// connect pour accelerer le bus 
	connect(widget.accelBusButton, SIGNAL( pressed() ), SLOT( accelBus() ));
	// connect pour afficher les informations du bus 
	connect(widget.button_viewbus, SIGNAL( pressed() ), SLOT( viewBusInfo() ));

	// Propriétés de l'arret
	// connect pour afficher les informations de l'arret 
	connect(widget.button_viewarret, SIGNAL( pressed() ), SLOT( viewStationInfo() ));

	// Menu évènements
	// Connect pour provoquer une émeute
	connect(widget.actionProvoquer_une_m_eute_dans_un_bus, SIGNAL( triggered() ), SLOT( actionEmeute() ));
	// Connect pour provoquer un bouchon
	connect(widget.actionCr_er_un_bouchon_sur_le_r_seau_routier, SIGNAL( triggered() ), SLOT( actionBouchon() ));
	// Connect pour provoquer une panne
	connect(widget.actionProvoquer_la_panne_d_un_bus, SIGNAL( triggered() ), SLOT( actionPanne() ));
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
			{ 	case 'S': Y +=  RTE_H; 
					break;
				case 'N': Y -=  RTE_H;
 					break;
				case 'E': X +=  RTE_W; 
					break;
				default: X -=  RTE_W; 
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
			if((rc->idRoad == atoi(xStation.getAttribute("road"))) && (rc->segment == (atoi(xStation.getAttribute("len"))/50)))
			{
				rc->idArret = atoi(xStation.getAttribute("id"));
				nomarret.push_back(xStation.getAttribute("nameA"));
				widget.cb_arret->insertItem(xStation.getAttribute("nameA"),atoi(xStation.getAttribute("id")));
				rc->setZValue(1.0);
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
//Bus_121055978 id=2 passengers=50 line=1:1,300;2,300;3,300;4,300;
//Bus_823281091 id=3 passengers=50 line=1:1,300;2,300;9,300;10,600;11,900;
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
}



/* Slots */

void Isibus::ajouterBus(int id)
{
        // Création de l'objet graphique
	BusSprite *newBus = new BusSprite( mAnimation.value(ID_BUS_LEFT_TO_RIGHT), field, ID_BUS_LEFT_TO_RIGHT, 2, 1, id );
        double dx = 0.0;
        double dy = 0.0;
        newBus->setVelocity( dx, dy );
        newBus->setPos(BG_W/2, BG_H/2 );
	newBus->setZValue(2.0);
	newBus->setFrame(1);
		
	buses.push_back(newBus);
	widget.cbBus->insertItem("Bus numero" + QString::number(id),id);
	newBus->show( );

}

void Isibus::addBus(){
	//bus_creation_dialog * BCDlg = new bus_creation_dialog(this);
	//BCDlg->show(this);
}

void Isibus::delBus(){
	worker->bus->SendMsg(" gui id= %d delete ", iDBusSelected);
	
}
void Isibus::slowBus(){
	worker->bus->SendMsg(" gui id= %d ralentir ", iDBusSelected);

}
void Isibus::accelBus(){
	worker->bus->SendMsg(" gui id= %d acceleter ", iDBusSelected);
}
void Isibus::viewBusInfo(){
	
	// recupération de l'id du bus courant
	iDBusSelected = widget.cbBus->currentItem();
	// envoie du message
}

void Isibus::viewStationInfo(){
	// recupération de l'id de l'arret courant
	iDarretSelected = widget.cb_arret->currentItem();
	// envoie du message
	worker->bus->SendMsg("gui getTimes station id= %d", iDarretSelected);
}

void Isibus::actionEmeute(){
	worker->bus->SendMsg(" gui id= %d emeute ", iDBusSelected);
}
void Isibus::actionBouchon(){
	worker->bus->SendMsg(" gui id= %d bouchon ", iDBusSelected);
}
void Isibus::actionPanne(){
	worker->bus->SendMsg(" gui id= %d panne ", iDBusSelected);
}


/* Ivy */

void Isibus::ajouterMessage(const QString& message)
{
	widget.lw_historique->addItem(message);
}

void Isibus::rafraichirArretInfo(const int &id,const int &id1,const int &h1,const QString &s1,const int &id2,const int &h2,const QString &s2,const int &id3,const int &h3,const QString &s3 )
{
	if(id1 != 0)
	{
		widget.gbBus1->setTitle("Bus numero" + QString::number(id1));
	}
	else
	{
		widget.gbBus1->setTitle("Pas de bus");
	}
	if(id2 != 0)
	{
		widget.gbBus1->setTitle("Bus numero" + QString::number(id2));
	}
	else
	{
		widget.gbBus1->setTitle("Pas de bus");
	}
	if(id3 != 0)
	{
		widget.gbBus1->setTitle("Bus numero" + QString::number(id3));
	}
	else
	{
		widget.gbBus1->setTitle("Pas de bus");
	}

	widget.lcdArretBus1->display(h1);	
	widget.lcdArretBus2->display(h2);	
	widget.lcdArretBus3->display(h3);
	widget.lcdArretBusStatus1->setText(s1);
	widget.lcdArretBusStatus2->setText(s2);
	widget.lcdArretBusStatus3->setText(s3);
}

void Isibus::bougerBus(const int &id, const int &ligne,const int &route,const int &segment,const int &capacite,const int &vitesse)
{

	int flag = 0;
	int x=0;
	int y=0;
	int vitessekh = (int)((vitesse * 3600 )/100);
	if(iDBusSelected == id)
	{
	widget.lcdn_ligne->display(ligne);
	widget.lcdn_numero->display(id);
	widget.lcdn_capacite->display(capacite);
	widget.lcdn_vitesse->display(vitessekh);
	widget.lcdn_route->display(route);
	widget.lcdn_segment->display(segment);
	widget.slowBusButton->setEnabled(true);
	widget.accelBusButton->setEnabled(true);
	widget.delBusButton->setEnabled(true);
	}
	foreach	(BusSprite * bs, buses){
			if(id == bs->id)
			{
				flag = 1;
				foreach	(RoadCase * rc, roadcaselist){
				cout<<"le bus X:"<< rc->x <<" Y:"<<rc->y<<"segment1:"<<(segment/50)<<"segment2:"<<rc->segment<<endl;
				if(rc->idRoad == route)
				{
					if(flag == 1)
					{
						x = rc->x;
						y = rc->y;
						switch(rc->direction)
						{ 	case 'S': y = y+(segment*0.54)-18; 
								  x += 8;

								bs->setFrame(1);
							break;
							case 'N': y = y-(segment*0.54)+18;
								bs->setFrame(2);
 								x += 8;
 							break;
							case 'E': x = x+(segment*0.66)-27; 
								bs->setFrame(0);
							break;
							case 'W': x = x-(segment*0.66)+27; 
								bs->setFrame(3);
							break;
						}
						flag = 2;
						
//Bus_121055978 id=2 passengers=50 line=1:1,300;2,300;3,300;4,300;
					}
				}

				bs->setPos(x,y);



				}
			}

		}

	if(flag == 0)
	{
		ajouterBus(id);
	}


}

/**********************************************************************************************
 * IvyWorker
***********************************************************************************************/

IvyWorker::IvyWorker(QObject * parent):QThread(parent)
{
	bus = new Ivy( "isiBusUI", "isiBusUI READY", this);
	bus->start(NULL);
	
	bus->BindMsg("(.*)", this);
	
	bus->BindMsg("^Bus id= ([0-9]+) line= ([0-9]+) pos= ([0-9]+), ([-]?[0-9]+) capacity= ([0-9]+) speed= ([0-9]+)", new msg::uiMvBus( this, qobject_cast<Isibus*>(parent) ));	

	bus->BindMsg("^Station id= ([0-9]+) time=(( [0-9]+, [0-9]+, [0-9]+;)*)", new msg::uiInfoStation( this, qobject_cast<Isibus*>(parent) ));	

	bus->BindMsg("(.*)", new msg::UiMsg( this, qobject_cast<Isibus*>(parent) ));


}


void IvyWorker::run()
{
	IvyC::IvyMainLoop ();
}
	
void IvyWorker :: MoveBus (const int &id, const int &ligne,const int &route,const int &segment,const int &capacite,const int &vitesse)
{	emit sigMoveBus(id, ligne, route, segment, capacite, vitesse);
}

void IvyWorker :: OnMessage(IvyApplication *app, int argc, const char **argv)
{
	emit sigAddMessage(QString(argv[0]));
}

void IvyWorker :: RefreshStationInfo(const int &id,const int &id1,const int &h1,const QString &s1,const int &id2,const int &h2,const QString &s2,const int &id3,const int &h3,const QString &s3 )
{
	emit sigRefreshStationInfo(id,id1,h1,s1,id2,h2,s2,id3,h3,s3);
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
