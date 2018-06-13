#include "ellipseitem.h"

#include <QDebug>
#include <QPainter>
#include <QCursor>

//=========================================================================================================
EllipseItem::EllipseItem(const QRectF &rect) : QGraphicsEllipseItem(rect)
{
    setPen(docPen);
    setBrush(docBrush);
    //attachGrabbers(rect);
    setFlags(ItemIsSelectable|ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    current_corner = 0;
}

//=========================================================================================================
QRectF EllipseItem::boundingRect() const
{
    QRectF rect = QGraphicsEllipseItem::boundingRect();

    //если элемент выбран, то добавятся углы и размер rect увеличиваем и смещаем его
    if(isSelected()){
        rect.setX(rect.x() - cornerRad);
        rect.setY(rect.y() - cornerRad);
        rect.setWidth(rect.width()+2*cornerRad);
        rect.setHeight(rect.height()+2*cornerRad);
    }
    return rect;
}

//=========================================================================================================
void EllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF re = QGraphicsEllipseItem::boundingRect();

    // если элемент выбран, то меняем цвет его границы
    if(isSelected()){
        painter->setPen(QPen(QColor(21,146,230)));
    }
    else{
        painter->setPen(this->pen());
    }

    painter->setBrush(this->brush());
    painter->drawEllipse(re);
    if(isSelected())
    {
        painter->setPen(QPen(QColor(21,146,230)));
        painter->setBrush(QBrush(QColor(0,0,0,0)));
        painter->drawRect(re);
    }
}

//=========================================================================================================
void EllipseItem::setScaleFactor(qreal factor)
{
    cornerRad /= factor;
}

void EllipseItem::cornerMove(GrabbingCorner *owner, qreal dx, qreal dy)
{
    QRectF tempRect = rect();
    QRectF tempTempRect = tempRect;

    float minSize = 10;

    switch(owner->getCornerType()){
    case Top: {
        tempTempRect.setTop(tempTempRect.top()+dy);
        break;
    }
    case Bottom: {
        tempTempRect.setBottom(tempTempRect.bottom()+dy);
        break;
    }
    case Right: {
        tempTempRect.setRight(tempTempRect.right()+dx);
        break;
    }
    case Left: {
        tempTempRect.setLeft(tempTempRect.left()+dx);
        break;
    }
    case TopLeft: {
        tempTempRect.setTopLeft(QPointF(tempTempRect.left()+dx,tempTempRect.top()+dy));
        break;
    }
    case BottomRight:{
        tempTempRect.setBottomRight(QPointF(tempTempRect.right()+dx,tempTempRect.bottom()+dy));
        break;
    }
    case TopRight:{
        tempTempRect.setTopRight(QPointF(tempTempRect.right()+dx,tempTempRect.top()+dy));
        break;
    }
    case BottomLeft: {
        tempTempRect.setBottomLeft(QPointF(tempTempRect.left()+dx,tempTempRect.bottom()+dy));
        break;
    }
}
    switch(owner->getCornerType())
{
    case Top: {
        if(tempTempRect.height() >= minSize)
            tempRect.setTop(tempRect.top()+dy);
        break;
    }
    case Bottom: {
        if(tempTempRect.height() >= minSize)
            tempRect.setBottom(tempRect.bottom()+dy);
        break;
    }
    case Right: {
        if(tempTempRect.width() >= minSize)
            tempRect.setRight(tempRect.right()+dx);
        break;
    }
    case Left: {
        if(tempTempRect.width() >= minSize)
            tempRect.setLeft(tempRect.left()+dx);
        break;
    }
    case TopLeft: {
        if(tempTempRect.height() >= minSize)
            tempRect.setTop(tempRect.top()+dy);
        if(tempTempRect.width() >= minSize)
            tempRect.setLeft(tempRect.left()+dx);
        break;
    }
    case BottomRight:{
        if(tempTempRect.height() >= minSize)
            tempRect.setBottom(tempRect.bottom()+dy);
        if(tempTempRect.width() >= minSize)
            tempRect.setRight(tempRect.right()+dx);
        break;
    }
    case TopRight:{
        if(tempTempRect.height() >= minSize)
            tempRect.setTop(tempRect.top()+dy);
        if(tempTempRect.width() >= minSize)
            tempRect.setRight(tempRect.right()+dx);
        break;
    }
    case BottomLeft: {
        if(tempTempRect.height() >= minSize)
            tempRect.setBottom(tempRect.bottom()+dy);
        if(tempTempRect.width() >= minSize)
            tempRect.setLeft(tempRect.left()+dx);
        break;
    }
}

    prepareGeometryChange();
    setRect( tempRect );
    update();
    updateCornersPosition();
}

//=========================================================================================================
void EllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    qDebug() << " MOUSE PRESS";
    if(ev->button() == Qt::LeftButton)
    {
        if(isSelected()){
            previous_pos = ev->pos();
            this->setCursor(Qt::SizeAllCursor);
        }
    }
    //QGraphicsItem::mousePressEvent(ev);
}

//=========================================================================================================
void EllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    qDebug() << " MOUSE RELEASE";
    if(ev->button() == Qt::LeftButton)
    {
        qDebug() << "EllipseItem::mouseReleaseEvent";
        this->setCursor(Qt::ArrowCursor);

            setSelected(true);
    }
    //QGraphicsItem::mouseReleaseEvent(ev);
}

//=========================================================================================================
void EllipseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
    if(isSelected()){
        auto d = ev->pos() - previous_pos;
        this->moveBy(d.x(), d.y());
    }
    QGraphicsItem::mouseMoveEvent(ev);
}

//=========================================================================================================
void EllipseItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
}

//=========================================================================================================
void EllipseItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent(event);
}

//=========================================================================================================
void EllipseItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF pt = event->pos();              // The current position of the mouse
    qreal drx = pt.x() - rect().right();    // Distance between the mouse and the right
    qreal dlx = pt.x() - rect().left();     // Distance between the mouse and the left

    qreal dty = pt.y() - rect().top();      // Distance between the mouse and the top
    qreal dby = pt.y() - rect().bottom();   // Distance between the mouse and the botto

    current_corner = 0;
    if( dby < 8 && dby > -8 && (dlx >= rect().width()/2-8 && dlx <= rect().width()/2+8)) current_corner |= Bottom;       // Top side
    if( dty < 4 && dty > -4 ) current_corner |= Top;    // Bottom side
    if( drx < 4 && drx > -4 ) current_corner |= Right;     // Right side
    if( dlx < 4 && dlx > -4 ) current_corner |= Left;      // Left side
    QGraphicsItem::hoverMoveEvent(event);
}

//=========================================================================================================
void EllipseItem::attachGrabbers(const QRectF &rect)
{
    grabbingCorners[Top] = new GrabbingCorner(QPointF(rect.left() + rect.width()/2, rect.top()), GrabbingCorner::Top, this); //top
    connect(grabbingCorners[Top], SIGNAL(signalMove(GrabbingCorner*,qreal,qreal)), this, SLOT(cornerMove(GrabbingCorner*,qreal,qreal)));

    grabbingCorners[Bottom] = new GrabbingCorner(QPointF(rect.left() + rect.width()/2, rect.bottom()), GrabbingCorner::Bottom, this); //bottom
    connect(grabbingCorners[Bottom], SIGNAL(signalMove(GrabbingCorner*,qreal,qreal)), this, SLOT(cornerMove(GrabbingCorner*,qreal,qreal)));

    grabbingCorners[Left] = new GrabbingCorner(QPointF(rect.left(), rect.top()+rect.height()/2), GrabbingCorner::Left, this); //left
    connect(grabbingCorners[Left], SIGNAL(signalMove(GrabbingCorner*,qreal,qreal)), this, SLOT(cornerMove(GrabbingCorner*,qreal,qreal)));

    grabbingCorners[Right] = new GrabbingCorner(QPointF(rect.right(), rect.top()+rect.height()/2), GrabbingCorner::Right, this); //right
    connect(grabbingCorners[Right], SIGNAL(signalMove(GrabbingCorner*,qreal,qreal)), this, SLOT(cornerMove(GrabbingCorner*,qreal,qreal)));

    grabbingCorners[TopLeft] = new GrabbingCorner(QPointF(rect.left(), rect.top()), GrabbingCorner::TopLeft,this); //top left
    connect(grabbingCorners[TopLeft], SIGNAL(signalMove(GrabbingCorner*,qreal,qreal)), this, SLOT(cornerMove(GrabbingCorner*,qreal,qreal)));

    grabbingCorners[TopRight] = new GrabbingCorner(QPointF(rect.right(), rect.top()), GrabbingCorner::TopRight, this); //top right
    connect(grabbingCorners[TopRight], SIGNAL(signalMove(GrabbingCorner*,qreal,qreal)), this, SLOT(cornerMove(GrabbingCorner*,qreal,qreal)));

    grabbingCorners[BottomLeft] = new GrabbingCorner(QPointF(rect.left(), rect.bottom()), GrabbingCorner::BottomLeft, this); //bottom left
    connect(grabbingCorners[BottomLeft], SIGNAL(signalMove(GrabbingCorner*,qreal,qreal)), this, SLOT(cornerMove(GrabbingCorner*,qreal,qreal)));

    grabbingCorners[BottomRight] = new GrabbingCorner(QPointF(rect.right(), rect.bottom()), GrabbingCorner::BottomRight, this); //bottom right
    connect(grabbingCorners[BottomRight], SIGNAL(signalMove(GrabbingCorner*,qreal,qreal)), this, SLOT(cornerMove(GrabbingCorner*,qreal,qreal)));
}

void EllipseItem::removeGrabbers()
{
    for(auto gc : grabbingCorners)
    {
       delete gc;
    }
}

void EllipseItem::updateCornersPosition()
{
    grabbingCorners[Top]->setPos(rect().left() + rect().width()/2, rect().top()); //top
    grabbingCorners[Bottom]->setPos(rect().left() + rect().width()/2, rect().bottom()); //bottom
    grabbingCorners[Left]->setPos(rect().left(), rect().top()+rect().height()/2); //left
    grabbingCorners[Right]->setPos(rect().right(), rect().top()+rect().height()/2); //right
    grabbingCorners[TopLeft]->setPos(rect().left(), rect().top()); //top left
    grabbingCorners[TopRight]->setPos(rect().right(), rect().top()); //top right
    grabbingCorners[BottomLeft]->setPos(rect().left(), rect().bottom()); //bottom left
    grabbingCorners[BottomRight]->setPos(rect().right(), rect().bottom()); //bottom right
}

QVariant EllipseItem::itemChange(EllipseItem::GraphicsItemChange change, const QVariant &value)
{
    qDebug() << "ITEM CHANGED";
    if(change == EllipseItem::ItemSelectedHasChanged)
    {
        qDebug() << "ITEM SELECION CHANGED";
        if(isSelected())
            attachGrabbers(rect());
        else
            removeGrabbers();
    }
    return QGraphicsItem::itemChange(change, value);
}
