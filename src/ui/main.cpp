 #include <QApplication>
 #include <QFont>
 #include <QGridLayout>
 #include <QPushButton>
 #include <QWidget>


#include "Isibus.h"

 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
     Isibus widget;
     widget.show();
     return app.exec();
 }
