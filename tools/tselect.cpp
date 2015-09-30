#include "tselect.h"
#include "glwidget.h"
#include "functions.h"

#include "mainwindow.h"

TSelect::TSelect(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{
    button->setText("Select");

    elements = new WidgetElements(0, 0, 0, 2, 0);
    elements->getMyCheckBox(0)->setText("Add");
    elements->getMyCheckBox(1)->setText("Remove");

    for(int i = 0; i < 2; i++)
    {
        connect(elements->getMyCheckBox(i), SIGNAL(handledClick(
            MyCheckBox*)), _mainWindow, SLOT(handleSelectClick(
                                          MyCheckBox*)));
        layout->addWidget(elements->getMyCheckBox(i), 0, i);
    }
    _widget->hide();

}

void TSelect::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = *_activeWidget;
    int i;
    WidgetElements *workWithElements = widget->
            getWorkWithElements();
    if(action == START)
    {
        QVector2D min, max;
        QVector2D startPosition = widget->getStartPosition();
        QVector2D currentPosition(event->x() - widget->
             getHalfWidth(), widget->getHalfHeight() - event->y());

        min.setX(qMin(startPosition.x(), currentPosition.x()) - 1);
        min.setY(qMin(startPosition.y(), currentPosition.y()) - 1);
        max.setX(qMax(startPosition.x(), currentPosition.x()) + 1);
        max.setY(qMax(startPosition.y(), currentPosition.y()) + 1);

        bool perspective = widget->getProjection() == PERSPECTIVE;
        widget->countFinalMatrix(perspective);
        int j;
        if(workWithElements->getRadioButton(0)->isChecked())
        {
            for(i = 0; i < widget->getModel()->vertexNumber; i++)
                model->vertex[i].setNewSelected(isSelected(widget->
                    getFinalMatrix(), model->vertex[i].getPosition(),
                                             perspective, min, max));
        }
        else
        {
         /*   for( i = 0; i < model->triangleNumber; i++ )
            {
                model->triangle[ i ].setNewSelected( false );
                for( j = 0; j < 3; j++ )
                {
                    if( isSelected( widget->getFinalMatrix(),
                        model->vertex[ model->triangle[ i ].
                        getIndex( j ) ].getPosition(), perspective,
                                    min, max ) )
                    {
                        model->triangle[ i ].setNewSelected( true );
                        break;
                    }
                }
            }*/
        }
        return;
    }
    if(action == DRAW)
    {
        VertexAndIndexData *data = widget->getToolData();
        data->vertices.resize(4);

        QVector2D min, max;
        QVector2D startPosition = widget->getStartPosition();
        QVector2D currentPosition = widget->getCurrentPosition();
        min.setX(qMin(startPosition.x(), currentPosition.x()) - 1);
        min.setY(qMin(startPosition.y(), currentPosition.y()) - 1);
        max.setX(qMax(startPosition.x(), currentPosition.x()) + 1);
        max.setY(qMax(startPosition.y(), currentPosition.y()) + 1);

        data->vertices[0].position = min;
        data->vertices[1].position.setX(max.x());
        data->vertices[1].position.setY(min.y());
        data->vertices[2].position = max;
        data->vertices[3].position.setX(min.x());
        data->vertices[3].position.setY(max.y());

        data->indices.resize(8);
        data->indices[0] = 0;
        data->indices[1] = 1;
        data->indices[2] = 1;
        data->indices[3] = 2;
        data->indices[4] = 2;
        data->indices[5] = 3;
        data->indices[6] = 3;
        data->indices[7] = 0;

        bool perspective = widget->getProjection() == PERSPECTIVE;
        widget->countFinalMatrix(perspective);
        int j;
        if(workWithElements->getRadioButton(0)->isChecked())
        {
            for(i = 0; i < widget->getModel()->vertexNumber; i++)
                model->vertex[i].setNewSelected( isSelected(
                    widget->getFinalMatrix(), model->vertex[ i ].
                           getPosition(), perspective, min, max ) );
        }
        else
        {
            for( i = 0; i < model->triangleNumber; i++ )
            {
                model->triangle[ i ].setNewSelected( false );
                for( j = 0; j < 3; j++ )
                {
                    if( isSelected( widget->getFinalMatrix(),
                        model->vertex[ model->triangle[ i ].
                        getIndex( j ) ].getPosition(), perspective,
                                    min, max ) )
                    {
                        model->triangle[ i ].setNewSelected( true );
                        break;
                    }
                }
            }
        }
    }
    if( action == STOP  )
    {
        WidgetElements *toolElements = elements;
        if( workWithElements->getRadioButton( 0 )->isChecked() )
        {
            int amount = 0;
            for( i = 0; i < model->vertexNumber; i++ )
            {
                amount += model->vertex[ i ].newSelected();
                if( !toolElements->getMyCheckBox( 0 )->isChecked()
                        && !toolElements->getMyCheckBox( 1 )->
                        isChecked() )
                    model->vertex[ i ].setSelected( model->vertex[
                                               i ].newSelected() );
                if( toolElements->getMyCheckBox( 0 )->isChecked() )
                {
                    if( !model->vertex[ i ].isSelected() )
                        model->vertex[ i ].setSelected( model->
                                    vertex[ i ].newSelected() );
                }
                if( toolElements->getMyCheckBox( 1 )->isChecked() )
                {
                    if( model->vertex[ i ].isSelected() )
                        model->vertex[ i ].setSelected( !model->
                                       vertex[ i ].newSelected() );
                }
                model->vertex[ i ].setNewSelected( false );
            }

        }
        else
        {
            for( i = 0; i < model->triangleNumber; i++ )
            {
                if( !toolElements->getMyCheckBox( 0 )->isChecked()
                        && !toolElements->getMyCheckBox( 1 )->
                        isChecked() )
                    model->triangle[ i ].setSelected(
                    model->triangle[ i ].newSelected() );
                if( toolElements->getMyCheckBox( 0 )->isChecked() )
                {
                    if( !model->triangle[ i ].isSelected() )
                        model->triangle[ i ].setSelected( model->
                                      triangle[ i ].newSelected() );
                }
                if( toolElements->getMyCheckBox( 1 )->isChecked() )
                {
                    if( model->triangle[ i ].isSelected() )
                        model->triangle[ i ].setSelected( !model->
                                      triangle[ i ].newSelected() );
                }
                model->triangle[ i ].setNewSelected( false );
            }
        }
    }
}