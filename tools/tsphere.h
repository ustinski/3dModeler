#ifndef TSPHERE_H
#define TSPHERE_H

#include "creatingtool.h"

#include "gui/myspinbox.h"

class TSphere : public CreatingTool
{
public:
    TSphere(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

private:
    MySpinBox *spinBox[6];
    MySpinBox *spinBoxRadius;

    QSpinBox *spinBoxSegmentsXY;
    QSpinBox *spinBoxSegmentsZ;

    QVector3D startPosition3D;
    QVector3D normal;
};

#endif // RSPHERE_H


    

