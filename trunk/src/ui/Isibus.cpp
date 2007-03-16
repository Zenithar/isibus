#include "Isibus.h"


#define IMG_BACKGROUND "pics/carte.png"

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
	field->setBackgroundBrush( pm );

	readSprites();

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
