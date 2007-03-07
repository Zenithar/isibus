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

#ifndef _MAINDLG_H_
#define _MAINDLG_H_

#include "ui_main.h"

#include <QtGui>

namespace isibus {
	
class MainDialog : public QMainWindow, public Ui::MainDlg
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    MainDialog(QWidget *parent=0, const char *name=0);

    /**
     * Default Destructor
     */
    virtual ~MainDialog();
};

}
#endif // _MAINDLG_H_
