
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
#include <QPoint>

#include <QtGui>

#include <qwidget.h>

#include <ui_Isibus.h>
#include <math.h>

#include "sprites.h"
#include "Ivycpp.h"
#include "IvyApplication.h"
#include "roadmap.h"

#include <pthread.h>

//#include "../base/xmlParser.h"
//#include "string.h"

//using namespace std;
namespace isibus
{
class IvyWorker : public QThread, public IvyApplicationCallback, public IvyMessageCallback, public IvyDirectMessageCallback {
	Q_OBJECT
	public:
		IvyWorker( QObject * parent = 0 );
		void run();
		
	signals:
		void sigAddMessage(const QString &message);
		void sigMoveBus(const int &id, const int &ligne,const int &route,const int &segment,const int &capacite,const int &vitesse);
		void sigRefreshStationInfo(const int &id,const int &id1,const int &h1,const QString &s1,const int &id2,const int &h2,const QString &s2,const int &id3,const int &h3,const QString &s3 );


	public:
		void OnApplicationConnected(IvyApplication *app);
		void OnApplicationDisconnected(IvyApplication *app);
		void OnMessage(IvyApplication *app, int argc, const char **argv);
		void OnDirectMessage (IvyApplication *app, int id, const char *arg );

		void MoveBus (const int &id, const int &ligne,const int &route,const int &segment,const int &capacite,const int &vitesse);
		void RefreshStationInfo(const int &id,const int &id1,const int &h1,const QString &s1,const int &id2,const int &h2,const QString &s2,const int &id3,const int &h3,const QString &s3 );
	public:
		Ivy *bus;	

};
	
class Isibus : public QMainWindow
{
	Q_OBJECT
public:

	Isibus(QWidget *parent=0);
	void readSprites();
	void timerEvent(QTimerEvent * );
	void wrapSprite( IsiSprite *s );
	void genererCarte( bool verbose);
	//vector<string> split(const string &sep,string text);

	void accelererBus(int coef);
	void ralenirBus(int coef);
	void supprimerBus(int IdBus);

	void ajoutPassagers(int NbPassagers);


	void initIvy();
	

private slots:
	void addBus(int id);
	void delBus();
	void slowBus();
	void accelBus();
	void viewBusInfo();
	void viewStationInfo();
	void actionEmeute();
	void actionBouchon();
	void actionPanne();


	void ajouterMessage(const QString &message);
	void rafraichirArretInfo(const int &id,const int &id1,const int &h1,const QString &s1,const int &id2,const int &h2,const QString &s2,const int &id3,const int &h3,const QString &s3 );
	void bougerBus(const int &id, const int &ligne,const int &route,const int &segment,const int &capacite,const int &vitesse);

private:
	QHash<int, QList<QPixmap>* > mAnimation;
	QGraphicsScene *field;
	int  mTimerId;
	QLinkedList<BusSprite*> buses;
	Ui_Isibus widget;
        QList<RoadCase *> roadcaselist ;
	
	IvyWorker* worker;
	pthread_mutex_t verrou;
	QList<QString> nomarret;
	int iDBusSelected;
	int iDarretSelected;
};

}
#endif
