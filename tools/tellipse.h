#ifndef TELLIPSE_H
#define TELLIPSE_H

#include "toolwithwidget.h"

#include "gui/myspinbox.h"
#include "gui/mycheckboxmw.h"

class TEllipse : public ToolWithWidget
{
public:
    TEllipse(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:

protected:
    QSpinBox *spinBoxSegments;
    MySpinBox *spinBox[6];
    MySpinBox *spinBoxRadius;

    MyCheckBoxMW *checkBoxCircle;

    QVector3D normal;
    bool ellipseFailed;

    QVector3D createNormal(const QVector3D &camRot);

private:
    void allocateCap(bool flip = false);
    void createCap(QVector4D rotatingVertex, double angle, const QMatrix4x4 &scaleAndTranslate);

    QVector3D startPosition3D;
};

#endif // TELLIPSE_H
