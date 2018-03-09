#include "editorscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QtMath>
EditorScene::EditorScene(QObject *parent) : QGraphicsScene(parent)
{
    currentMode = Pointer;
    currentKeyMode = None;
    currentItemColor = Qt::black;
}

void EditorScene::setWidth(QString text)
{
    if(selectedItem != 0){
        selectedItem->boundingRect().setWidth(text.toInt());
        QGraphicsRectItem *temp = dynamic_cast<QGraphicsRectItem*>(selectedItem);
        temp->setRect(selectedItem->x(), selectedItem->y(), text.toInt(), selectedItem->boundingRect().height());
        qDebug() << text << "SIGNAL";
    }

}

void EditorScene::setHeight(int num)
{
    if(selectedItem != 0)
        selectedItem->boundingRect().setHeight(num);
}

void EditorScene::setX(int num)
{
    if(selectedItem != 0)
        selectedItem->boundingRect().setX(num);
}

void EditorScene::setY(int num)
{
    if(selectedItem != 0)
        selectedItem->boundingRect().setY(num);
}

void EditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() != Qt::LeftButton)
        return;

    sx = mouseEvent->buttonDownScenePos(Qt::LeftButton).x();
    sy = mouseEvent->buttonDownScenePos(Qt::LeftButton).y();
    if(currentMode == Pointer){
        auto item = this->itemAt(mouseEvent->scenePos().x(), mouseEvent->scenePos().y(),QTransform());
        selectItem(item);
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}


void EditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() != Qt::LeftButton){

        cx = mouseEvent->scenePos().x();
        cy = mouseEvent->scenePos().y();

        int width = 0;
        int height = 0;

        if(currentKeyMode == Shift){
           int wt = cx - sx;
           int ht = cy - sy;
           bool wplus = wt >= 0;
           bool hplus = ht >= 0;
           if(qAbs(wt) < qAbs(ht)){
               width = wt;
               height = hplus ? qAbs(wt) : -qAbs(wt);
           }
           else{
               width = wplus ? qAbs(ht) : -qAbs(ht);
               height = ht;
           }

           qDebug() << "WIDTH = " << width;
           qDebug() << "HEIGHT = " << height;
        }
        else{
            width = cx - sx;
            height = cy - sy;
        }

        if(temp_item != 0)
            this->removeItem(temp_item);


        QRect rect(sx,sy,width,height);
        switch(currentMode)
        {
            case RectItem:
                drawRect(rect);
                break;
            case EllipseItem:
                drawEllipse(rect);
                break;
            case LineItem:
                drawLine(rect);
                break;
            default:
                break;
        }
    }
    if(currentMode == Pointer){
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void EditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() != Qt::LeftButton)
        return;
    selectItem(temp_item);
    temp_item = 0;

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void EditorScene::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() & Qt::ShiftModifier)
        currentKeyMode = Shift;

    if(event->key() == Qt::Key_Delete){
        if(selectedItem != 0)
            this->removeItem(selectedItem);
    }

}

void EditorScene::keyReleaseEvent(QKeyEvent *event)
{
    currentKeyMode = None;
}

void EditorScene::drawRect(QRect rect)
{
    temp_item = new QGraphicsRectItem(rect.normalized());
    QGraphicsRectItem *temp = dynamic_cast<QGraphicsRectItem*>(temp_item);
    temp->setRect(0,0,100,100);
    temp_item->setFlag(QGraphicsItem::ItemIsMovable);
    temp_item->setFlag(QGraphicsItem::ItemIsSelectable);
    this->addItem(temp);
    temp_item->boundingRect().setSize(QSize(rect.width(), rect.height()));
    temp_item->boundingRect().setWidth(rect.width());
    temp_item->boundingRect().setHeight(rect.height());
    emit widthChanged(temp_item->boundingRect().width());
    emit heightChanged(temp_item->boundingRect().height());
}

void EditorScene::drawEllipse(QRect rect)
{
    temp_item = new QGraphicsEllipseItem(rect.normalized());
    temp_item->setFlag(QGraphicsItem::ItemIsMovable);
    temp_item->setFlag(QGraphicsItem::ItemIsSelectable);
    this->addItem(temp_item);
    emit widthChanged(qAbs(cx-sx));
    emit heightChanged(qAbs(cy-sy));
}

void EditorScene::drawLine(QRect rect)
{
    if(currentKeyMode == Shift){

        //а вот так мы вычисляем ближайший кратный)
        int angle  = qRadiansToDegrees(atan2(cy-sy,cx-sx));
        bool minus = angle < 0;
        int multiple = 45;
        int real_angle = qAbs(angle);
        if(real_angle % multiple != 0){
            int tmp = real_angle - real_angle / multiple * multiple;
            angle = tmp > multiple/2.0 ? (real_angle / multiple + 1) * multiple : real_angle / multiple * multiple;
            angle = minus ? -angle : angle;
        }

        QLineF line;
        line.setP1(QPointF(sx,sy));
        line.setAngle(-angle);
        line.setLength(qSqrt(qPow(cx-sx,2) + qPow(cy-sy,2)));
        temp_item = new QGraphicsLineItem(line);
        temp_item->setFlag(QGraphicsItem::ItemIsMovable);
        this->addItem(temp_item);
    }
    else{
        temp_item = new QGraphicsLineItem(sx,sy,cx,cy);
        temp_item->setFlag(QGraphicsItem::ItemIsMovable);
        temp_item->setFlag(QGraphicsItem::ItemIsSelectable);
        this->addItem(temp_item);
    }
    emit widthChanged(qAbs(cx-sx));
    emit heightChanged(qAbs(cy-sy));
}

void EditorScene::selectItem(QGraphicsItem *item)
{
    selectedItem = 0;
    foreach(QGraphicsItem *item, this->selectedItems()){
        item->setSelected(false);
    }

    if(item != 0){
        item->setSelected(true);
        selectedItem = item;
    }
}
