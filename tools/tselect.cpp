#include "tselect.h"
#include "glwidget.h"
#include "mainwindow.h"
#include "journal.h"
#include "target.h"
#include "types.h"
#include "trianglecontainer.h"

using namespace Model;
using namespace Journal;

TSelect::TSelect() : ToolWithWidget()
{
    button->setText("Select");
    for(int i = 0; i < 2; i++) checkBox[i] = new MyCheckBox;
    checkBox[0]->setText("Add");
    checkBox[1]->setText("Remove");
    for(int i = 0; i < 2; i++)
    {
        checkBox[i]->setNeighbor(checkBox[!i]);
        layout->addWidget(checkBox[i], 0, i, 1, 2);
    }

    ignoreBackfacing = new QCheckBox;
    ignoreBackfacing->setText("Ignore Backfacing");
    ignoreBackfacing->setChecked(true);
    layout->addWidget(ignoreBackfacing, 1, 0, 1, 3);

    _widget->hide();

    _shift = checkBox[0];
    _ctrl = checkBox[1];
}

void TSelect::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = Workspace::activeWidget();

    switch(action)
    {
    case START:
    {
        _busy = true;
        QVector2D min, max;
        QVector2D currentPosition(event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y());
        countMinAndMax(&min, &max, currentPosition);

        _select(min, max);
        break;
    }
    case DRAW:
    {
        VertexAndIndexData &data = widget->getToolData();
        vector <VertexData_Color> &vertices = data.vertices;
        vector <GLuint> &indices = data.indices;
        vertices.resize(4);

        QVector2D min, max;
        countMinAndMax(&min, &max, widget->getCurrentPosition());

        vertices[0].position = min;
        vertices[1].position.setX(max.x());
        vertices[1].position.setY(min.y());
        vertices[2].position = max;
        vertices[3].position.setX(min.x());
        vertices[3].position.setY(max.y());

        indices.resize(8);
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 1;
        indices[3] = 2;
        indices[4] = 2;
        indices[5] = 3;
        indices[6] = 3;
        indices[7] = 0;

        //exept drawing here we check is vertices are selected (inside rectangle)
        //this should be in EXECUTE, but here "min" and "max" are counted already
        _select(min, max);
        break;
    }
    case STOP:
    {
        _busy = false;
        Journal::newRecord(EDIT);

        if(Target::isVertex())
        {
            for(int i = 0; i < vertex().size(); i++)
            {
                if(!vertex()[i].exists()) continue;

                bool newSelected = vertex()[i].newSelected();
                vertex()[i].setNewSelected(false);

                if(!checkBox[0]->isChecked() && !checkBox[1]->isChecked()) vertex().setSelected(i, newSelected);
                if(checkBox[0]->isChecked() && newSelected) vertex().setSelected(i, true);
                if(checkBox[1]->isChecked() && newSelected) vertex().setSelected(i, false);
            }
        }
        else
        {
            for(tr_it it = triangle().begin(); it != triangle().end(); it++) {
                if(!it->exists()) continue;
                bool newSelected = it->newSelected();
                it->setNewSelected(false);

                if(!checkBox[0]->isChecked() && !checkBox[1]->isChecked()) triangle().setSelected(it, newSelected);
                if(checkBox[0]->isChecked() && newSelected) triangle().setSelected(it, true);
                if(checkBox[1]->isChecked() && newSelected) triangle().setSelected(it, false);
            }
        }

        Journal::submit();
        break;
    }
    }
}

void TSelect::countMinAndMax(QVector2D *min, QVector2D *max, const QVector2D &currentPosition)
{
    GLWidget *widget = Workspace::activeWidget();
    const QVector2D &startPosition = widget->getStartPosition();

    min->setX(qMin(startPosition.x(), currentPosition.x()) - 2);
    min->setY(qMin(startPosition.y(), currentPosition.y()) - 2);
    max->setX(qMax(startPosition.x(), currentPosition.x()) + 2);
    max->setY(qMax(startPosition.y(), currentPosition.y()) + 2);
}

void TSelect::_select(const QVector2D &min, const QVector2D &max)
{
    GLWidget *widget = Workspace::activeWidget();
    int i;

    widget->countFinalMatrix();
    int j;
    if(Target::isVertex())
    {
        for(i = 0; i < vertex().size(); i++) if(vertex()[i].exists()) vertex()[i].setNewSelected(widget->isSelected(vertex()[i].position(), min, max));
    }
    else
    {
        vector <QVector2D> vertexOnScreen(vertex().size());
        for(i = 0; i < vertex().size(); i++) if(vertex()[i].exists()) widget->fromWorldToScreen(&vertexOnScreen[i], vertex()[i].position());


        bool selected[vertex().size()];
        for(i = 0; i < vertex().size(); i++) if(vertex()[i].exists()) selected[i] = widget->isSelected(vertex()[i].position(), min, max);

        int k;
        QVector2D rectanglePoints[4];
        for(i = 0; i < 4; i++) rectanglePoints[i] = QVector2D(i % 2 ? max.x() : min.x(), i / 2 ? max.y() : min.y());
        QVector2D edge;
        QVector2D fromVertexToCurrent;
        double normal;

        QVector2D minT, maxT;

        for(tr_it it = triangle().begin(); it != triangle().end(); it++) {
            if(!it->exists()) continue;
            it->setNewSelected(false);

            normal = QVector3D::crossProduct(vertexOnScreen[it->index(1)] - vertexOnScreen[it->index(0)], vertexOnScreen[it->index(2)] - vertexOnScreen[it->index(0)]).z();

            if(ignoreBackfacing->isChecked() && normal < 0) continue;

            //check if triangle and rectangle can have common points
            minT = maxT = vertexOnScreen[it->index(0)];
            for(j = 1; j < 3; j++) {
                const QVector2D &v = vertexOnScreen[it->index(j)];
                if(v.x() < minT.x()) minT.setX(v.x());
                if(v.y() < minT.y()) minT.setY(v.y());
                if(v.x() > maxT.x()) maxT.setX(v.x());
                if(v.y() > maxT.y()) maxT.setY(v.y());
            }
            if(min.x() > maxT.x() || minT.x() > max.x() || min.y() > maxT.y() || minT.y() > max.y()) continue;

            //check if some vertex of triangle is inside rectangle
            for(j = 0; j < 3; j++) {
                if(selected[it->index(j)]) {
                    it->setNewSelected(true);
                    break;
                }
            }
            if(it->newSelected()) continue;

            //check if some point of rectangle (like vertex) is inside triangle
            for(j = 0; j < 4; j++) {
                for(k = 0; k < 3; k++) {
                    edge = vertexOnScreen[it->index((k + 1) % 3)] - vertexOnScreen[it->index(k)];
                    fromVertexToCurrent = rectanglePoints[j] - vertexOnScreen[it->index(k)];
                    if(QVector3D::crossProduct(edge, fromVertexToCurrent).z() * normal <= 0) break;
                }
                if(k == 3) it->setNewSelected(true);
            }
            if(it->newSelected()) continue;

            //check if edges of triangle and rectangle intersect
            //loop through triangle edges
            for(j = 0; j < 3; j++)
            {
                // edge - j, (j + 1) % 3e
                minT = maxT = vertexOnScreen[it->index(j)];
                const QVector2D &v = vertexOnScreen[it->index((j + 1) % 3)];
                if(v.x() < minT.x()) minT.setX(v.x());
                if(v.y() < minT.y()) minT.setY(v.y());
                if(v.x() > maxT.x()) maxT.setX(v.x());
                if(v.y() > maxT.y()) maxT.setY(v.y());

                if(minT.x() > max.x() || maxT.x() < min.x() || minT.y() > max.y() || maxT.y() < min.y()) continue;

                //loop through coordinates: [0] ~ .x(), [1] ~ .y()
                for(int l = 0; l < 2; l++)
                {
                    //loop through rectangle sides (0 - lower, 1 - higher)
                    for(k = 0; k < 2; k ++)
                    {
                        int recPoint = k * (2 - l);

                        if(minT[l] >= rectanglePoints[recPoint][l] || maxT[l] <= rectanglePoints[recPoint][l]) continue;

                        QVector2D &v1 = vertexOnScreen[it->index(j)];
                        QVector2D &v2 = vertexOnScreen[it->index((j + 1) % 3)];

                        bool shouldBeSelected = false;
                        if(v1[!l] == v2[!l])
                        {
                            if(v1[!l] > min[!l] && v1[!l] < max[!l]) shouldBeSelected = true;
                        }
                        else
                        {
                            double tan = (v1[l] - v2[l]) / (v1[!l] - v2[!l]);
                            if(tan != 0)
                            {
                                double b = v1[l] - tan * v1[!l];
                                double x = (rectanglePoints[recPoint][l] - b) / tan;
                                if(x > min[!l] && x < max[!l]) shouldBeSelected = true;
                            }
                        }
                        if(!shouldBeSelected) continue;

                        it->setNewSelected(true);
                        break;
                    }
                }
            }
        }
    }
}
