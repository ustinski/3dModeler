#include "tcylinder.h"
#include "glwidget.h"
#include "mainwindow.h"

#include "gui/mylabel.h"

TCylinder::TCylinder(MainWindow *mainWindow) : TEllipse(mainWindow)
{
    button->setText("Cylinder");
    finalButton->setText("Create Cylinder");

    MyLabel *height = new MyLabel("Height", 70);
    spinBoxHeight = new MySpinBox;
    spinBoxHeight->setMinimum(0);
    spinBoxHeight->setValue(1);

    layout->addWidget(height, 7, 0, 1, 2);
    layout->addWidget(spinBoxHeight, 7, 2, 1, 2);

    _hasStage2 = true;
    _widget->show();
}

void TCylinder::function(Action action, QMouseEvent *event)
{
    if(action == DRAW) return;

    //cylinder's STAGE2 has much code from ellipse's STOP
    if(action != STOP && action != STAGE2) TEllipse::function(action, event);

    GLWidget *widget = *_activeWidget;
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexSize = vertex.size();
    int segments = spinBoxSegments->value();
    int i;


    switch(action)
    {
    //all START and most of FINAL (creating first cap) was done in
    //    TEllipse::function(action, event);
    case FINAL:
    {
        if(ellipseFailed) return;
        if(spinBoxHeight->value() == 0)
        {
            vertex.resize(vertexSize - segments - 1);
            triangle.erase(triangle.end() - segments, triangle.end());
            return;
        }
        createWallsAndSecondCap(true);
        for(i = 0; i <= segments; i++) vertex[vertexSize + i].setSelected(true);
        break;
    }
    //if(!_stage2) is done in TEllipse::function(action, event);
    case EXECUTE:
    {
        if(_stage2)
        {
            Projection projection = widget->getProjection();
            double dy = (widget->getHalfHeight() - event->y() - widget->getLastPosition().y()) / double(100);

            for(i = -segments - 1; i < 0; i++) vertex[vertexSize + i].addToPosition(normal * dy);

            //v1 and v2 are not parallel vectors in cap, [v1, v2] is normal to cap
            QVector3D v1 = vertex[vertexSize - segments - 1].getPosition() - vertex[vertexSize - 1].getPosition();
            QVector3D v2 = vertex[vertexSize - segments].getPosition() - vertex[vertexSize - 1].getPosition();

            //flip if needed
            if(QVector3D::dotProduct(normal, vertex[vertexSize - segments - 2].getPosition() - vertex[vertexSize - 1].getPosition()) * QVector3D::dotProduct(normal, QVector3D::crossProduct(v1, v2)) > 0)
            {
                QVector3D temp;
                for(i = 0; i < segments / 2; i++)
                {
                    temp = vertex[vertexSize - segments - 1 + i].getPosition();
                    vertex[vertexSize - segments - 1 + i] = vertex[vertexSize - 2 - i];
                    vertex[vertexSize - 2 - i].setPosition(temp);
                    temp = vertex[vertexSize - 2 * segments - 2 + i].getPosition();
                    vertex[vertexSize - 2 * segments - 2 + i] = vertex[vertexSize - segments - 3 - i];
                    vertex[vertexSize - segments - 3 - i].setPosition(temp);
                }
            }
        }
        break;
    }
    case STOP:
    {
        vertexSize = vertex.size();
        //if height == 0 we remove cylinder (2 * segments + 2 vertices and 4 *
        //    segments triangles
        if(vertex[vertexSize - 1] == vertex[vertexSize - segments - 2])
        {
            vertex.resize(vertexSize - 2 * segments - 2);
            triangle.erase(triangle.end() - 4 * segments, triangle.end());
        }
        else for(i = 1; i < 2 * segments + 3; i++) vertex[vertexSize - i].setSelected(true, false);
        widget->setToolIsOn(false);
        setStage2(false);
        widget->setMouseTracking(false);
        break;
    }
    case STAGE2:
    {
        ellipseFailed = false;
        TEllipse::function(STOP, event);
        if(ellipseFailed) return;

        widget->setToolIsOn(true);
        setStage2(true);

        createWallsAndSecondCap(false);

        for(i = 0; i <= segments; i++) vertex[vertexSize + i].setNewSelected(true);

        widget->setMouseTracking(true);
    }
    }
}

void TCylinder::createWallsAndSecondCap(bool final)
{
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexSize = vertex.size();
    int segments = spinBoxSegments->value();
    int i;

    QVector3D height = final ? normal * spinBoxHeight->value() : QVector3D(0, 0, 0);
    //add vertices for second cap
    vertex.resize(vertexSize + segments + 1);
    //second cap
    for(i = 0; i <= segments; i++) vertex[vertexSize + i].setPosition(vertex[vertexSize - segments - 1 + i].getPosition() + height);

    //add triangles for walls and second cap

    //cap
    for(i = 0; i < segments - 1; i++) triangle.push_back({vertexSize + i, vertexSize + i + 1, vertexSize + segments});
    triangle.push_back({vertexSize + segments - 1, vertexSize, vertexSize + segments});

    //wall
    for(i = 0; i < segments - 1; i++)
    {
        triangle.push_back({vertexSize + i + 1, vertexSize + i, vertexSize - segments - 1 + i});
        triangle.push_back({vertexSize - segments - 1 + i, vertexSize - segments + i, vertexSize + i + 1});
    }
    triangle.push_back({vertexSize, vertexSize + segments - 1, vertexSize - 2});
    triangle.push_back({vertexSize - 2, vertexSize - segments - 1, vertexSize});
}
