//
// C++ Implementation: busgraphicsitem
//
// Description: 
//
//
// Author: Thibault Normand <thibault.normand@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QtGui>

#include "busgraphicsitem.h"

namespace isibus {

BusGraphicsItem::BusGraphicsItem(QGraphicsItem *parent)
	: QGraphicsItem(parent),angle(0), speed(0), posx(0), posy(0)
{
	pixBus = QPixmap("bus.png");
}


BusGraphicsItem::~BusGraphicsItem()
{
}

QRectF BusGraphicsItem::boundingRect() const
{
	return QRectF();
}

void BusGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->drawPixmap(posx, posy, pixBus);
}

void BusGraphicsItem::timerEvent(QTimerEvent *)
{
}

}
