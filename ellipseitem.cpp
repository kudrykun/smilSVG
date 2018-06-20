#include "ellipseitem.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QCursor>

//=========================================================================================================
EllipseItem::EllipseItem(const QRectF &rect) : QGraphicsEllipseItem(rect)
{
    setRect(rect);
    setPen(currentPen);
    setBrush(currentBrush);
    setFlags(ItemIsSelectable|ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    current_corner = 0;
}

//=========================================================================================================
EllipseItem *EllipseItem::copy()
{
    EllipseItem *newItem = new EllipseItem(this->rect());
    newItem->setPen(this->getPen());
    newItem->setPos(this->pos());
    newItem->setBrush(this->getBrush());
    return newItem;
}

//=========================================================================================================
QRectF EllipseItem::boundingRect() const
{
    QRectF rect = this->rect();
    rect.adjust(-currentPen.width()/2,-currentPen.width()/2,currentPen.width()/2,currentPen.width()/2);
    if(isSelected()){
        rect.adjust(-cornerRad/2, - cornerRad/2, cornerRad/2, cornerRad/2);
    }
    return rect;
}

//=========================================================================================================
void EllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF re = this->rect();

    painter->setBrush(currentBrush);
    painter->setPen(currentPen);
    painter->drawEllipse(re);

    if(isSelected())
    {
        painter->setPen(QPen(QColor(21,146,230)));
        painter->setBrush(QBrush(QColor(0,0,0,0)));
        auto rect = boundingRect();
        rect.adjust(cornerRad/2,cornerRad/2, -cornerRad/2, -cornerRad/2);
        painter->drawRect(rect);
    }

    if(debug_mode)
    {
        painter->setBrush(QBrush(Qt::transparent));
        painter->setPen(QPen(Qt::red));
        painter->drawRect(rect());

        painter->setBrush(QBrush(Qt::transparent));
        painter->setPen(QPen(Qt::blue));
        painter->drawRect(boundingRect());
    }
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

//=========================================================================================================
void EllipseItem::setPen(const QPen &pen)
{
    currentPen = pen;
}

//=========================================================================================================
void EllipseItem::setBrush(const QBrush &brush)
{
    currentBrush = brush;
}

//=========================================================================================================
QPen EllipseItem::getPen()
{
    return currentPen;
}

//=========================================================================================================
QBrush EllipseItem::getBrush()
{
    return currentBrush;
}

//=========================================================================================================
void EllipseItem::setStrokeWidth(float w)
{
    currentPen.setWidthF(w);
}

//=========================================================================================================
void EllipseItem::setStrokeOpacity(float op)
{
    if(op < 0)
        op = 0;
    if(op > 1)
        op = 1;
    auto new_color = currentPen.color();
    new_color.setAlphaF(op);
    currentPen.setColor(new_color);
}

//=========================================================================================================
void EllipseItem::setStrokeColor(QColor stroke_color)
{
    auto new_color = stroke_color;
    new_color.setAlpha(currentPen.color().alpha());
    currentPen.setColor(new_color);
}

//=========================================================================================================
void EllipseItem::setStrokeLineCap(Qt::PenCapStyle capStyle)
{
    currentPen.setCapStyle(capStyle);
}

//=========================================================================================================
void EllipseItem::setStrokeLineJoin(Qt::PenJoinStyle joinStyle)
{
    currentPen.setJoinStyle(joinStyle);
}

//=========================================================================================================
void EllipseItem::setStrokeDashoffset(qreal offset)
{
    if(offset >= 0)
        currentPen.setDashOffset(offset);
}

//=========================================================================================================
void EllipseItem::setStrokeDasharray(const QVector<qreal> &pattern)
{
    if(!pattern.empty())
        currentPen.setDashPattern(pattern);
}

//=========================================================================================================
void EllipseItem::setScaleFactor(qreal factor)
{
    cornerRad /= factor;
}

//=========================================================================================================
void EllipseItem::cornerMove(GrabbingCorner *owner, qreal dx, qreal dy)
{
    QRectF tempRect = rect();
    QRectF tempTempRect = tempRect;

    float minSize = 1;

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
    setRect(QRectF(0,0,tempRect.width(), tempRect.height()));
    setPos(QPointF(tempRect.x(), tempRect.y()) + pos());
    update();
    updateCornersPosition();
    qDebug() << this->rect() << " " << this->pos();
}

//=========================================================================================================
void EllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        if(isSelected()){
            previous_pos = ev->pos();
            this->setCursor(Qt::SizeAllCursor);
        }
    }
}

//=========================================================================================================
void EllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        this->setCursor(Qt::ArrowCursor);

            setSelected(true);
    }
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
void EllipseItem::attachGrabbers()
{
    auto rect = boundingRect();
    rect.adjust(cornerRad/2,cornerRad/2, -cornerRad/2, -cornerRad/2);

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

//=========================================================================================================
void EllipseItem::removeGrabbers()
{
    for(auto gc : grabbingCorners)
    {
       delete gc;
    }
}

//=========================================================================================================
void EllipseItem::updateCornersPosition()
{
    auto rect = boundingRect();
    rect.adjust(cornerRad/2,cornerRad/2, -cornerRad/2, -cornerRad/2);

    grabbingCorners[Top]->setPos(rect.left() + rect.width()/2, rect.top()); //top
    grabbingCorners[Bottom]->setPos(rect.left() + rect.width()/2, rect.bottom()); //bottom
    grabbingCorners[Left]->setPos(rect.left(), rect.top()+rect.height()/2); //left
    grabbingCorners[Right]->setPos(rect.right(), rect.top()+rect.height()/2); //right
    grabbingCorners[TopLeft]->setPos(rect.left(), rect.top()); //top left
    grabbingCorners[TopRight]->setPos(rect.right(), rect.top()); //top right
    grabbingCorners[BottomLeft]->setPos(rect.left(), rect.bottom()); //bottom left
    grabbingCorners[BottomRight]->setPos(rect.right(), rect.bottom()); //bottom right
}

//=========================================================================================================
QVariant EllipseItem::itemChange(EllipseItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == EllipseItem::ItemSelectedHasChanged)
    {
        if(isSelected())
            attachGrabbers();
        else{
            removeGrabbers();
            qDebug() << "REMOVE GABBERS";
        }
    }
    return QGraphicsItem::itemChange(change, value);
}
