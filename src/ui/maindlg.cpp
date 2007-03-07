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

#include "maindlg.h"

#include "cc.h"

namespace isibus {

MainDialog::MainDialog(QWidget *parent, const char *name):QMainWindow(parent)
{
	setupUi(this);
}

MainDialog::~MainDialog()
{
}

}
