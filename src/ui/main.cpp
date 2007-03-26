 #include <QApplication>
 #include <QFont>
 #include <QGridLayout>
 #include <QPushButton>
 #include <QWidget>


#include "Isibus.h"

pthread_t threadIvy;
using namespace isibus;
/**
* Démarre le centre de contrôle.
* @return etat de l'execution.
*/
void * mainLoop(void *ptr)
{	
	IvyC::IvyMainLoop ();
	return 0;
}

 int main(int argc, char *argv[])
 {
	QApplication app(argc, argv);
	Isibus widget;
	widget.show();

	pthread_create(& threadIvy,
			NULL,
			mainLoop,
			NULL);

	return app.exec();
 }
