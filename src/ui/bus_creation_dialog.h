#ifndef ISIBUS_H
#define ISIBUS_H

#include <QtGui>

#include <qwidget.h>

#include <ui_bus_creation_dialog.h>


//#include "Isibus.h"

namespace isibus
{


class bus_creation_dialog : public QDialog
{
	Q_OBJECT
public:
	bus_creation_dialog(QWidget *parent=0);

private:
	Ui_bus_creation_dialog widget;

};

}

#endif
