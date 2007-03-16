
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

//#include "../base/xmlParser.h"
//#include "string.h"

//using namespace std;

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

private slots:
	void addBus(  );

private:
	QHash<int, QList<QPixmap>* > mAnimation;
	QGraphicsScene *field;
	int  mTimerId;
	QLinkedList<Bus*> buses;
	Ui_Isibus widget;
        QList<RoadCase *> roadcaselist ;
};

#endif
