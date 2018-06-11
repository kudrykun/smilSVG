#include "editorview.h"
#include <QApplication>
#include <QDebug>
#include <QScrollBar>
//=========================================================================================================
EditorView::EditorView(EditorScene *scene, QObject *parent)
{
    setRenderHint(QPainter::RenderHint::Antialiasing);
    setScene(scene);
    connect(this, SIGNAL(scaleChanged(qreal)), scene, SLOT(updateScale(qreal)));
    //setMouseTracking(true);
}

//=========================================================================================================
//переопределяем для зума сцены
void EditorView::wheelEvent(QWheelEvent *ev)
{

    //скейлим только если нажат ctrl
    if( QApplication::keyboardModifiers() && Qt::ControlModifier){
        if(ev->angleDelta().y() > 0){
            current_scale *= 1.05;
            this->scale(1.05, 1.05);
            emit scaleChanged(1.05);
        }
        else{
            //чтобы не слишком убого смотрелось
            if(current_scale > 0.4){
                current_scale *= 0.95;
                this->scale(0.95, 0.95);
                emit scaleChanged(0.95);
            }
        }

    }
}

//=========================================================================================================
void EditorView::mousePressEvent(QMouseEvent *ev)
{
    if(space_pressed){
        start = ev->pos();

    }

    QGraphicsView::mousePressEvent(ev);
}

//=========================================================================================================
void EditorView::mouseMoveEvent(QMouseEvent *ev)
{
    if(space_pressed){
        QPointF end = ev->pos();
        QPointF tr = end - start;
        start = end;

        this->horizontalScrollBar()->setValue( this->horizontalScrollBar()->value() - tr.x());
        this->verticalScrollBar()->setValue(  this->verticalScrollBar()->value() - tr.y());


    }
    QGraphicsView::mouseMoveEvent(ev);
}

//=========================================================================================================
void EditorView::mouseReleaseEvent(QMouseEvent *ev)
{
    QGraphicsView::mouseReleaseEvent(ev);
}

//=========================================================================================================
void EditorView::keyPressEvent(QKeyEvent *ev)
{
    switch(ev->key()){
        case Qt::Key_Space:
            space_pressed = true;
            break;
        default:
            QGraphicsView::keyPressEvent(ev);
    }
}

//=========================================================================================================
void EditorView::keyReleaseEvent(QKeyEvent *ev)
{
    switch(ev->key()){
        case Qt::Key_Space:
            space_pressed = false;
            break;
        default:
            QGraphicsView::keyReleaseEvent(ev);
    }
}
