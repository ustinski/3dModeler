#include "ttriangle.h"
#include "glwidget.h"
#include "mainwindow.h"

TTriangle::TTriangle(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{
    button->setText("Triangle");
    finalButton = new QPushButton("Cancel");
    finalButton->setMaximumWidth(150);

    layout->addWidget(finalButton, 0, 0, 1, 2 );
    connect(finalButton, SIGNAL(clicked()), _mainWindow, SLOT(final()));
    _widget->hide();
}

void TTriangle::function(Action action, QMouseEvent *event)
{
    if(action != START && action != FINAL) return;

    int i;
    vector <Vertex> &vertex = model->getVertex();

    if(action == START)
    {
        GLWidget *widget = *_activeWidget;
        vector <Triangle> &triangle = model->getTriangle();

        for(i = 0; i < newTriangle.size(); i++) vertex[newTriangle[i]].setNewSelected(true);

        QVector2D min, max;
        const QVector2D &startPosition = widget->getStartPosition();
        QVector2D currentPosition(event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y());
        min.setX(qMin(startPosition.x(), currentPosition.x()) - 5);
        min.setY(qMin(startPosition.y(), currentPosition.y()) - 5);
        max.setX(qMax(startPosition.x(), currentPosition.x()) + 5);
        max.setY(qMax(startPosition.y(), currentPosition.y()) + 5);

        widget->countFinalMatrix();
        int j;
        for(i = 0; i < vertex.size(); i++)
        {
            bool selectedBefore = false;
            for(j = 0; j < newTriangle.size(); j++)
            {
                if(i == newTriangle[j])
                {
                    selectedBefore = true;
                    break;
                }
            }
            if(!selectedBefore && widget->isSelected(vertex[i].getPosition(), min, max))
            {
                newTriangle.push_back(i);
                vertex[i].setNewSelected(true);
            }
        }
        if(newTriangle.size() == 3)
        {
            triangle.push_back(newTriangle.data());
            for(i = 0; i < 3; i++) vertex[newTriangle[i]].setSelected(true, false);
            newTriangle.clear();
        }
    }
    else
    {
        for(i = 0; i < newTriangle.size(); i++) vertex[newTriangle[i]].setNewSelected(false);
        newTriangle.clear();
    }
}

void TTriangle::setActive(bool value)
{
    if(!value) function(FINAL, 0);
    ToolWithWidget::setActive(value);
}
