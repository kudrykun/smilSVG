#include "grabbingcorner.h"
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QCursor>
//=========================================================================================================
GrabbingCorner::GrabbingCorner(QPointF pos, CornerType type, QGraphicsItem *paremtItem, QObject *parent) : QObject(parent)
{
    this->setPos(pos);
    previous_pos = pos;
    this->setParentItem(paremtItem);
    this->setBrush(QBrush(fillColor));
    this->setPen(QPen(strokeColor));
    this->setRect(-cornerRad/2, -cornerRad/2, cornerRad, cornerRad);
    setAcceptHoverEvents(true);
    setFlags(ItemIsMovable|ItemSendsGeometryChanges);
    corner_type = type;
}

//=========================================================================================================
void GrabbingCorner::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    //setPreviousPos(ev->scenePos());
    QGraphicsItem::mousePressEvent(ev);
}

//=========================================================================================================
void GrabbingCorner::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
    auto dx = ev->pos().x();
    auto dy = ev->pos().y();
    setPreviousPos(ev->scenePos());
    emit signalMove(this,dx,dy);
}

//=========================================================================================================
void GrabbingCorner::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    QGraphicsItem::mouseReleaseEvent(ev);
}

//=========================================================================================================
QPointF GrabbingCorner::getPreviousPos() const
{
    return previous_pos;
}

//=========================================================================================================
void GrabbingCorner::setPreviousPos(const QPointF pos)
{
    previous_pos = pos;
}

//=========================================================================================================
void GrabbingCorner::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setBrush(filHoverColor);

    switch(corner_type)
    {
    case Top:
        this->setCursor(Qt::SizeVerCursor);
        break;
    case Bottom:
        this->setCursor(Qt::SizeVerCursor);
        break;
    case Right:
        this->setCursor(Qt::SizeHorCursor);
        break;
    case Left:
        this->setCursor(Qt::SizeHorCursor);
        break;
    case TopLeft:
        this->setCursor(Qt::SizeFDiagCursor);
        break;
    case BottomRight:
        this->setCursor(Qt::SizeFDiagCursor);
        break;
    case TopRight:
        this->setCursor(Qt::SizeBDiagCursor);
        break;
    case BottomLeft:
        this->setCursor(Qt::SizeBDiagCursor);
        break;
    }
}

//=========================================================================================================
void GrabbingCorner::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setBrush(fillColor);
    parentItem()->setCursor(Qt::ArrowCursor);
}



