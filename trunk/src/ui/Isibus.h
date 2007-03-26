
#ifndef ISIBUS_H
#define ISIBUS_H

#include <QMenu>
#include <QPainter>
#include <QStyle>
#include <QList>
#include <QHeaderView>
#include <QObject>
#include <QTreeWidget>
#include <QStringList>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QGraphicsScene>
#include <QPixmap>
#include <QLinkedList>

#include <QtGui>

#include <qwidget.h>

#include <ui_Isibus.h>
#include <math.h>

#include "sprites.h"
#include "Ivycpp.h"
#include "IvyApplication.h"
#include "roadmap.h"
#include "runnable.h"

#include <pthread.h>

//#include "../base/xmlParser.h"
//#include "string.h"

//using namespace std;
namespace isibus
{
class Isibus : public QMainWindow, public IvyApplicationCallback, public IvyMessageCallback, public IvyDirectMessageCallback
{
	Q_OBJECT
public:

	Isibus(QWidget *parent=0);
	void readSprites();
	void timerEvent(QTimerEvent * );
	void wrapSprite( IsiSprite *s );
	void genererCarte( bool verbose);
	//vector<string> split(const string &sep,string text);

	void initIvy();

	Ivy *bus;

	void OnApplicationConnected(IvyApplication *app);
	void OnApplicationDisconnected(IvyApplication *app);
	void OnMessage(IvyApplication *app, int argc, const char **argv);
	void OnDirectMessage (IvyApplication *app, int id, const char *arg );
	
	void ajouterMessage(const char* message);

private slots:
	void addBus(  );

private:
	QHash<int, QList<QPixmap>* > mAnimation;
	QGraphicsScene *field;
	int  mTimerId;
	QLinkedList<BusSprite*> buses;
	Ui_Isibus widget;
        QList<RoadCase *> roadcaselist ;
	
	pthread_mutex_t verrou;
};
}
#endif
