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

    qDebug() << "MOVE EVENT";
    qDebug() << "curr " << ev->scenePos() << " prev " << previous_pos;
    auto dx = ev->scenePos().x() - previous_pos.x();
    auto dy = ev->scenePos().y() - previous_pos.y();

    switch(corner_type)
    {
    case Top:
        qDebug() << "TOP";
        dx = 0;
        moveBy(dx,dy);
        break;
    case Bottom:
        qDebug() << "Bottom";
        dx = 0;
        moveBy(dx,dy);
        break;
    case Right:
        qDebug() << "Right";
        dy = 0;
        moveBy(dx,0);
        break;
    case Left:
        qDebug() << "Left";
        dy = 0;
        moveBy(dx,0);
        break;
    case TopLeft:
        qDebug() << "TopLeft";
        moveBy(dx,dy);
        break;
    case BottomRight:
        qDebug() << "BottomRight";
        moveBy(dx,dy);
        break;
    case TopRight:
        qDebug() << "TopRight";
        moveBy(dx,dy);
        break;
    case BottomLeft:
        qDebug() << "BottomLeft";
        moveBy(dx,dy);
        break;
    }
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
    qDebug() << "grabber hover enter";
    Q_UNUSED(event);
    this->setBrush(filHoverColor);

    switch(corner_type)
    {
    case Top:
        qDebug() << "TOP";
        this->setCursor(Qt::SizeVerCursor);
        break;
    case Bottom:
        qDebug() << "Bottom";
        this->setCursor(Qt::SizeVerCursor);
        break;
    case Right:
        qDebug() << "Right";
        this->setCursor(Qt::SizeHorCursor);
        break;
    case Left:
        qDebug() << "Left";
        this->setCursor(Qt::SizeHorCursor);
        break;
    case TopLeft:
        qDebug() << "TopLeft";
        this->setCursor(Qt::SizeFDiagCursor);
        break;
    case BottomRight:
        qDebug() << "BottomRight";
        this->setCursor(Qt::SizeFDiagCursor);
        break;
    case TopRight:
        qDebug() << "TopRight";
        this->setCursor(Qt::SizeBDiagCursor);
        break;
    case BottomLeft:
        qDebug() << "BottomLeft";
        this->setCursor(Qt::SizeBDiagCursor);
        break;
    }
}

//=========================================================================================================
void GrabbingCorner::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "grabber hover leave";
    Q_UNUSED(event);
    this->setBrush(fillColor);
    parentItem()->setCursor(Qt::ArrowCursor);
}



