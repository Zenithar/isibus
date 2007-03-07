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
