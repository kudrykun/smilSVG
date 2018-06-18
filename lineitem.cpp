#include "lineitem.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QCursor>

//=========================================================================================================
LineItem::LineItem(const QLineF &line) : QGraphicsLineItem(line)
{
    setLine(line);
    setPen(currentPen);
    setFlags(ItemIsSelectable|ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    current_corner = 0;
}

//=========================================================================================================
QRectF LineItem::boundingRect() const
{
    QRectF rect = QRectF(this->line().p1(),this->line().p2()).normalized();
    rect.adjust(-currentPen.width()/2,-currentPen.width()/2,currentPen.width()/2,currentPen.width()/2);
    if(isSelected()){
        rect.adjust(-cornerRad/2, - cornerRad/2, cornerRad/2, cornerRad/2);
    }
    return rect;
}

//=========================================================================================================
void LineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QLineF line = this->line();
    painter->setPen(currentPen);
    painter->drawLine(line);

    if(isSelected())
    {
        painter->setPen(QPen(QColor(21,146,230)));
        painter->drawLine(line);
        auto rect = boundingRect();
        rect.adjust(cornerRad/2,cornerRad/2, -cornerRad/2, -cornerRad/2);
        painter->drawRect(rect);
    }

    if(debug_mode)
    {
        painter->setBrush(QBrush(Qt::transparent));
        painter->setPen(QPen(Qt::red));
        painter->drawRect(QRectF(this->line().p1(),this->line().p2()).normalized());

        painter->setBrush(QBrush(Qt::transparent));
        painter->setPen(QPen(Qt::blue));
        painter->drawRect(boundingRect());
    }
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

//=========================================================================================================
void LineItem::setPen(const QPen &pen)
{
    currentPen = pen;
}

//=========================================================================================================
QPen LineItem::getPen()
{
    return currentPen;
}

//=========================================================================================================
void LineItem::setStrokeWidth(float w)
{
    currentPen.setWidthF(w);
}

//=========================================================================================================
void LineItem::setStrokeOpacity(float op)
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
void LineItem::setStrokeColor(QColor stroke_color)
{
    auto new_color = stroke_color;
    new_color.setAlpha(currentPen.color().alpha());
    currentPen.setColor(new_color);
}


//=========================================================================================================
void LineItem::setStrokeLineCap(Qt::PenCapStyle capStyle)
{
    currentPen.setCapStyle(capStyle);
}

//=========================================================================================================
void LineItem::setStrokeLineJoin(Qt::PenJoinStyle joinStyle)
{
    currentPen.setJoinStyle(joinStyle);
}

//=========================================================================================================
void LineItem::setStrokeDashoffset(qreal offset)
{
    if(offset >= 0)
        currentPen.setDashOffset(offset);
}

//=========================================================================================================
void LineItem::setStrokeDasharray(const QVector<qreal> &pattern)
{
    if(!pattern.empty())
        currentPen.setDashPattern(pattern);
}

//=========================================================================================================
void LineItem::setScaleFactor(qreal factor)
{
    cornerRad /= factor;
}

//=========================================================================================================
void LineItem::cornerMove(GrabbingCorner *owner, qreal dx, qreal dy)
{
    QLineF line = this->line();
    switch(owner->getCornerType()){
        case TopLeft: {
            line.setP1(QPointF(line.p1().x()+dx,line.p1().y()+dy));
            break;
        }
        case BottomRight:{
            line.setP2(QPointF(line.p2().x()+dx,line.p2().y()+dy));
            break;
        }
    }

    qDebug() << line.p1() << " " << line.p2();
    prepareGeometryChange();
    setLine(line);
    update();
    updateCornersPosition();
}

//=========================================================================================================
void LineItem::mousePressEvent(QGraphicsSceneMouseEvent *ev)
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
void LineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        this->setCursor(Qt::ArrowCursor);

            setSelected(true);
    }
}

//=========================================================================================================
void LineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
    if(isSelected()){
        auto d = ev->pos() - previous_pos;
        this->moveBy(d.x(), d.y());
    }
    QGraphicsItem::mouseMoveEvent(ev);
}

//=========================================================================================================
void LineItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
}

//=========================================================================================================
void LineItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent(event);
}

//=========================================================================================================
void LineItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
//    QPointF pt = event->pos();              // The current position of the mouse
//    qreal drx = pt.x() - line().x1();    // Distance between the mouse and the right
//    qreal dlx = pt.x() - line().x2();     // Distance between the mouse and the left

//    qreal dty = pt.y() - line().y1();      // Distance between the mouse and the top
//    qreal dby = pt.y() - line().y2();   // Distance between the mouse and the botto

//    current_corner = 0;
//    if( dby < 8 && dby > -8 && (dlx >= rect().width()/2-8 && dlx <= rect().width()/2+8)) current_corner |= Bottom;       // Top side
//    if( dty < 4 && dty > -4 ) current_corner |= Top;    // Bottom side
//    if( drx < 4 && drx > -4 ) current_corner |= Right;     // Right side
//    if( dlx < 4 && dlx > -4 ) current_corner |= Left;      // Left side
//    QGraphicsItem::hoverMoveEvent(event);
}

//=========================================================================================================
void LineItem::attachGrabbers()
{
    auto rect = boundingRect();
    rect.adjust(cornerRad/2,cornerRad/2, -cornerRad/2, -cornerRad/2);

    grabbingCorners[0] = new GrabbingCorner(QPointF(rect.left(), rect.top()), GrabbingCorner::TopLeft,this); //top left
    connect(grabbingCorners[0], SIGNAL(signalMove(GrabbingCorner*,qreal,qreal)), this, SLOT(cornerMove(GrabbingCorner*,qreal,qreal)));

    grabbingCorners[1] = new GrabbingCorner(QPointF(rect.right(), rect.bottom()), GrabbingCorner::BottomRight, this); //bottom right
    connect(grabbingCorners[1], SIGNAL(signalMove(GrabbingCorner*,qreal,qreal)), this, SLOT(cornerMove(GrabbingCorner*,qreal,qreal)));
}

//=========================================================================================================
void LineItem::removeGrabbers()
{
    for(auto gc : grabbingCorners)
    {
       delete gc;
    }
}

//=========================================================================================================
void LineItem::updateCornersPosition()
{
    //auto rect = boundingRect();
    //rect.adjust(cornerRad/2,cornerRad/2, -cornerRad/2, -cornerRad/2);


    grabbingCorners[0]->setPos(line().p1()); //top left
    grabbingCorners[1]->setPos(line().p2()); //bottom right
}

//=========================================================================================================
QVariant LineItem::itemChange(LineItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == LineItem::ItemSelectedHasChanged)
    {
        if(isSelected())
            attachGrabbers();
        else
            removeGrabbers();
    }
    return QGraphicsItem::itemChange(change, value);
}

