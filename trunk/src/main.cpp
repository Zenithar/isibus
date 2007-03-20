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

#include "cc.h"

using namespace isibus;

void printUsage()
{
	cout << "Usage : isibus_cc <file>" << endl;
	cout << "\t" << "<file> : fichier xml decrivant la ville." << endl;
	cout << endl;
	cout << " * Par defaut roadmap.xml sera chargé." << endl;
}

int main(int argc, char* argv[])
{	
	if(argc < 2)
	{
		printUsage();
		exit(-1);
	}
	
	ControlCenter cc;
	cc.loadMap(argv[1], true);
	
	return cc.mainLoop();
}
