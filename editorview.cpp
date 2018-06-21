#include "editorview.h"
#include <QApplication>
#include <QDebug>
#include <QScrollBar>
#include "rectitem.h"
#include "ellipseitem.h"
#include "lineitem.h"
//=========================================================================================================
EditorView::EditorView(EditorScene *scene, QObject *parent)
{
    setRenderHint(QPainter::RenderHint::Antialiasing);
    setScene(scene);
    connect(this, SIGNAL(scaleChanged(qreal)), scene, SLOT(updateScale(qreal)));
    setDragMode(QGraphicsView::RubberBandDrag);
    //setMouseTracking(true);
}

//=========================================================================================================
void EditorView::deleteItem()
{
    auto items = scene()->selectedItems();
    for(auto &i : items)
        scene()->removeItem(i);
    qDebug() << "DELE SLOTE";
}

//=========================================================================================================
void EditorView::duplicateItem()
{
    qDebug() << scene()->selectedItems().size();
    for(auto &i : scene()->selectedItems())
    {
        bool duplicated = false;

        if(!duplicated){
            RectItem* item = dynamic_cast<RectItem*>(i);
            if(item != 0)
            {
                RectItem* newItem = item->copy();
                newItem->moveBy(10,10);
                scene()->addItem(newItem);

                i->setSelected(false);
                newItem->setSelected(true);
                duplicated = true;
            }
        }

        if(!duplicated){
            EllipseItem* item = dynamic_cast<EllipseItem*>(i);
            if(item != 0)
            {
                EllipseItem* newItem = item->copy();
                newItem->moveBy(10,10);
                scene()->addItem(newItem);

                i->setSelected(false);
                newItem->setSelected(true);
                duplicated = true;
            }
        }

        if(!duplicated){
            LineItem* item = dynamic_cast<LineItem*>(i);
            if(item != 0)
            {
                LineItem* newItem = item->copy();
                newItem->moveBy(10,10);
                scene()->addItem(newItem);

                i->setSelected(false);
                newItem->setSelected(true);
                duplicated = true;
            }
        }
    }
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
    if(item_tool_state != None && lmb_pressed){
        qDebug() << "MOUSE MOVE IN VIEW";
        switch(item_tool_state)
        {
            case RectTool:{
                if(drawing_item != 0)
                    scene()->removeItem(drawing_item);
                drawing_rect = QRectF(this->mapToScene(previous_mouse_pos.toPoint()), this->mapToScene(ev->pos())).normalized();
                drawing_item = new RectItem(QRectF(0,0,drawing_rect.width(),drawing_rect.height()));
                drawing_item->setPos(drawing_rect.x(),drawing_rect.y());
                scene()->addItem(drawing_item);

                return;
            }

            case EllipseTool:{
                if(drawing_item != 0)
                    scene()->removeItem(drawing_item);
                drawing_rect = QRectF(this->mapToScene(previous_mouse_pos.toPoint()), this->mapToScene(ev->pos())).normalized();
                drawing_item = new EllipseItem(QRectF(0,0,drawing_rect.width(),drawing_rect.height()));
                drawing_item->setPos(drawing_rect.x(),drawing_rect.y());
                scene()->addItem(drawing_item);
                return;
            }

            case LineTool:{
                if(drawing_item != 0)
                    scene()->removeItem(drawing_item);
                drawing_item = new LineItem(QLineF(this->mapToScene(previous_mouse_pos.toPoint()), this->mapToScene(ev->pos())));
                scene()->addItem(drawing_item);
                return;
            }
        }
    }
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
    if(item_tool_state != None && lmb_pressed){
        if(drawing_item != 0){
            if(item_tool_state != LineTool)
                drawing_item->setPos(drawing_rect.x(), drawing_rect.y());
            drawing_item = nullptr;
        }
    }

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
