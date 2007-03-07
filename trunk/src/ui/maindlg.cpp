//
// C++ Implementation: maindlg
//
// Description: 
//
//
// Author: Thibault Normand <thibault.normand@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "maindlg.h"

#include "cc.h"

namespace isibus {

MainDialog::MainDialog(QWidget *parent, const char *name):QMainWindow(parent)
{
	setupUi(this);
	
	ControlCenter cc;
	cc.startControl();
}

MainDialog::~MainDialog()
{
}

}
