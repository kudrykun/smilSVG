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
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse); //для зума под положение курсора
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
    if(ev->button() == Qt::LeftButton)
    {
        lmb_pressed = true;
        previous_mouse_pos = ev->pos();
    }
    if(space_pressed && lmb_pressed)
    {
        QApplication::restoreOverrideCursor();
        QApplication::setOverrideCursor(Qt::ClosedHandCursor);
    }
    if(!space_pressed)
        QGraphicsView::mousePressEvent(ev);
}

//=========================================================================================================
void EditorView::mouseMoveEvent(QMouseEvent *ev)
{
    if(space_pressed && lmb_pressed){
        auto tr = ev->pos() - previous_mouse_pos;
        this->horizontalScrollBar()->setValue( this->horizontalScrollBar()->value() - tr.x());
        this->verticalScrollBar()->setValue(  this->verticalScrollBar()->value() - tr.y());
    }
    previous_mouse_pos = ev->pos();
    if(!space_pressed)
        QGraphicsView::mouseMoveEvent(ev);
}

//=========================================================================================================
void EditorView::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        lmb_pressed = false;
    }

    if(!lmb_pressed && space_pressed)
    {
        QApplication::restoreOverrideCursor();
        QApplication::setOverrideCursor(Qt::OpenHandCursor);
    }
    if(!space_pressed)
        QGraphicsView::mouseReleaseEvent(ev);
}

//=========================================================================================================
void EditorView::keyPressEvent(QKeyEvent *ev)
{
    switch(ev->key()){
        case Qt::Key_Space:
            if(!ev->isAutoRepeat()){
                space_pressed = true;
                QApplication::restoreOverrideCursor();
                QApplication::setOverrideCursor(Qt::OpenHandCursor);
            }
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
            if(!ev->isAutoRepeat()){
                space_pressed = false;
                QApplication::restoreOverrideCursor();
            }
            break;
        default:
            QGraphicsView::keyReleaseEvent(ev);
    }
}
