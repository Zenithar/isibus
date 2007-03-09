//
// C++ Interface: busgraphicsitem
//
// Description: 
//
//
// Author: Thibault Normand <thibault.normand@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ISIBUSBUSGRAPHICSITEM_H
#define ISIBUSBUSGRAPHICSITEM_H

#include <QGraphicsItem>

namespace isibus {

/**
Element graphique représentant un bus sur la carte.

	@author Thibault Normand <thibault.normand@gmail.com>
*/
class BusGraphicsItem : public QObject, public QGraphicsItem
{
Q_OBJECT
public:
	BusGraphicsItem(QGraphicsItem *parent = 0);

    	~BusGraphicsItem();
	
	QRectF boundingRect() const;
	
    	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
	       QWidget *widget);

protected:
	void timerEvent(QTimerEvent *event);

private:
	qreal angle;
	qreal speed;
	int  posx;
	int  posy;
	
	QPixmap pixBus;
		
};

}

#endif
