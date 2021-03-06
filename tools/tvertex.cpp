#include "tvertex.h"
#include "glwidget.h"
#include "mainwindow.h"
#include "journal.h"

#include "gui/mylabel.h"

using namespace Journal;

TVertex::TVertex() : CreatingTool()
{
    button->setText("Vertex");
    finalButton = new QPushButton("Create Vertex");

    int i;
    MyLabel *label[3];
    for(i = 0; i < 3; i++ )
    {
        spinBox[i] = new MySpinBox;
        label[i] = new MyLabel(QString('X' + i) + ':', 50);
        layout->addWidget(label[i], i, 0);
        layout->addWidget(spinBox[i], i, 1);
    }
    layout->addWidget(finalButton, 3, 0, 1, 2);
    connect(finalButton, SIGNAL(clicked()), this, SLOT(final()));
    _widget->hide();
}

void TVertex::function(Action action, QMouseEvent *event)
{
    using namespace Model;
    if(action == DRAW) return;

    GLWidget *widget = Workspace::activeWidget();
    widget->countFinalInverseMatrix();
    if(action == START || action == FINAL)
    {
        Journal::newRecord(CREATE);

        QVector3D newVertex;
        if(action == START)
        {
            _busy = true;
            widget->fromScreenToWorld(&newVertex, event);
        }
        else for(int i = 0; i < 3; i++) newVertex[i] = spinBox[i]->value();
        ver.clear();
        ver.push_back(vertex().push(newVertex));
        vertex()[ver[0]].setNewSelected(true);
        if(action == FINAL) action = STOP;
    }
    if(action == EXECUTE) widget->fromScreenToWorld(&vertex()[ver[0]].position(), event);
    if(action == STOP)
    {
        _busy = false;
        vertex()[ver[0]].setSelected(true, false);
        Journal::submit();
    }
}
