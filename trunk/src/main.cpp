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

#include <QApplication>
#include <QtGui>

#include "maindlg.h"
#include "cc.h"

using namespace isibus;

int main(int argc, char* argv[])
{	
	ControlCenter cc;
	cc.loadMap("roadmap.xml", true);
	
	return cc.mainLoop();
}
