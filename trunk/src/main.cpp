#include <QApplication>
#include <QtGui>

#include "maindlg.h"

using namespace isibus;

int main(int argc, char* argv[])
{	
	QApplication app(argc, argv);	
	
	MainDialog *fenetre = new MainDialog();
	fenetre->show();
	
	return app.exec();
}
