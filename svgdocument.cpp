#include "svgdocument.h"
#include <QDebug>
#include <QPainter>
#include <QCursor>

//=========================================================================================================
SvgDocument::SvgDocument(const QRectF &rect) : QGraphicsRectItem(rect)
{
    setPen(docPen);
    setBrush(docBrush);
    attachGrabbers(rect);
    setFlags(ItemIsSelectable|ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    current_corner = 0;
    qDebug() << this->boundingRect();
}

//=========================================================================================================
QRectF SvgDocument::boundingRect() const
{
    QRectF rect = QGraphicsRectItem::boundingRect();

    //если элемент выбран, то добавятся углы и размер rect увеличиваем и смещаем его
    if(selected){
        rect.setX(rect.x() - cornerRad);
        rect.setY(rect.y() - cornerRad);
        rect.setWidth(rect.width()+2*cornerRad);
        rect.setHeight(rect.height()+2*cornerRad);
    }
    return rect;
}

//=========================================================================================================
void SvgDocument::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF re = QGraphicsRectItem::boundingRect();

    // если элемент выбран, то меняем цвет его границы
    if(selected){
        painter->setPen(QPen(QColor(21,146,230)));
    }
    else{
        painter->setPen(this->pen());
    }

    painter->setBrush(this->brush());
    painter->drawRect(re);
}

//=========================================================================================================
void SvgDocument::setScaleFactor(qreal factor)
{
    cornerRad /= factor;
    qDebug() << "СЛОТ В СЛОТЕ БРАТАН " << cornerRad;
}

void SvgDocument::cornerMove(GrabbingCorner *owner, qreal dx, qreal dy)
{
    qDebug() << "MOVE CORNER!";
    switch(owner->getCornerType())
    {
    case Top: {
        qDebug() << "MOVE TOP";
        QRectF tempRect = rect();
        tempRect.setTop(tempRect.top()+dy);
        prepareGeometryChange();
        setRect( tempRect );
        update();
        break;
    }
    case Bottom: {
        qDebug() << " MOVE Bottom";
        QRectF tempRect = rect();
        tempRect.setBottom(tempRect.bottom()+dy);
        prepareGeometryChange();
        setRect( tempRect );
        update();
        break;
    }
    case Right: {
        qDebug() << "MOVE Right";
        QRectF tempRect = rect();
        tempRect.setRight(tempRect.right()+dx);
        prepareGeometryChange();
        setRect( tempRect );
        update();
        break;
    }
    case Left: {
        qDebug() << "MOVE Left";
        QRectF tempRect = rect();
        qDebug() << "RECT POS " << tempRect.x() << " " << tempRect.y();
        tempRect.setLeft(tempRect.left()+dx);
        qDebug() << "RECT POS " << tempRect.x() << " " << tempRect.y();
        //tempRect.translate(-dx , 0 );
        //tempRect.translate(rect().width() - tempRect.width(),0);
        prepareGeometryChange();
        setRect( tempRect );
        update();
        qDebug() << "RECT POS " << tempRect.x() << " " << tempRect.y();
        break;
    }
    case TopLeft: {
        qDebug() << "MOVE TopLeft";
        QRectF tempRect = rect();
        //tempRect.setLeft(tempRect.left()+dx);
        //tempRect.setTop(tempRect.top()+dy);
        tempRect.setTopLeft(QPointF(tempRect.left()+dx,tempRect.top()+dy));
        //tempRect.translate(dx , 0 );
        prepareGeometryChange();
        setRect( tempRect );
        update();
        break;
    }
    case BottomRight:{
        qDebug() << "MOVE BottomRight";
        QRectF tempRect = rect();
        //tempRect.setRight(tempRect.right()+dx);
        //tempRect.setBottom(tempRect.bottom()+dy);
        tempRect.setBottomRight(QPointF(tempRect.right()+dx,tempRect.bottom()+dy));
        prepareGeometryChange();
        setRect( tempRect );
        update();
        break;
    }
    case TopRight:{
        qDebug() << "MOVE TopRight";
        QRectF tempRect = rect();
        //tempRect.setRight(tempRect.right()+dx);
        //tempRect.setTop(tempRect.top()+dy);
        tempRect.setTopRight(QPointF(tempRect.right()+dx,tempRect.top()+dy));
        prepareGeometryChange();
        setRect( tempRect );
        update();
        break;
    }
    case BottomLeft:
        qDebug() << "MOVE BottomLeft";
        QRectF tempRect = rect();
        //tempRect.setLeft(tempRect.left()+dx);
        //tempRect.setBottom(tempRect.bottom()+dy);
        tempRect.setBottomLeft(QPointF(tempRect.left()+dx,tempRect.bottom()+dy));
        //tempRect.translate(dx , 0 );
        prepareGeometryChange();
        setRect( tempRect );
        //rect().
        update();
        break;
    }
    updateCornersPosition();
}

//=========================================================================================================
void SvgDocument::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
    //qDebug() << "MOVE CORNER";
    QGraphicsItem::mouseMoveEvent(ev);
}

//=========================================================================================================
void SvgDocument::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "hoverEnter";
    QGraphicsItem::hoverEnterEvent(event);
}

//=========================================================================================================
void SvgDocument::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "hoverLeave";
    QGraphicsItem::hoverLeaveEvent(event);
}

//=========================================================================================================
void SvgDocument::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF pt = event->pos();              // The current position of the mouse
    qreal drx = pt.x() - rect().right();    // Distance between the mouse and the right
    qreal dlx = pt.x() - rect().left();     // Distance between the mouse and the left

    qreal dty = pt.y() - rect().top();      // Distance between the mouse and the top
    qreal dby = pt.y() - rect().bottom();   // Distance between the mouse and the botto

    current_corner = 0;
    //if( dby < 7 && dby > -7 ) current_corner |= Top;       // Top side
    //if( dty < 7 && dty > -7 ) current_corner |= Bottom;    // Bottom side
    //if( drx < 7 && drx > -7 ) current_corner |= Right;     // Right side
    //if( dlx < 7 && dlx > -7 ) current_corner |= Left;      // Left side

    //if((qAbs(dlx)/2 < 8 || qAbs(drx)/2 < 8) && qAbs(dty) < 8) current_corner = Top;
    ///if(qAbs(drx) < 8 && (qAbs(dby)/2 < 8 || qAbs(dty)/2 < 8)) current_corner = Right;
    //if((qAbs(dlx)/2 < 8 || qAbs(drx)/2 < 8) && qAbs(dby) < 8) current_corner = Bottom;
    //if(qAbs(dlx) < 8 && (qAbs(dby)/2 < 8 || qAbs(dty)/2 < 8)) current_corner = Left;

    //if(pt.x() < rect().topRight().x()+4 &&
            //pt.x() > rect().topRight().x()-4 &&
           // pt.y() < rect().topRight().y()+4 &&
            //pt.y() > rect().topRight().y()-4) current_corner = TopRight;

    if( dby < 8 && dby > -8 && (dlx >= rect().width()/2-8 && dlx <= rect().width()/2+8)) current_corner |= Bottom;       // Top side
    if( dty < 4 && dty > -4 ) current_corner |= Top;    // Bottom side
    if( drx < 4 && drx > -4 ) current_corner |= Right;     // Right side
    if( dlx < 4 && dlx > -4 ) current_corner |= Left;      // Left side

    qDebug() << rect().width()/2;

    //if(qAbs(dlx) < 8 && qAbs(dby) < 8) current_corner = TopLeft;
    //if(qAbs(drx) < 8 && qAbs(dby) < 8) current_corner = TopRight;
    //if(qAbs(drx) < 8 && qAbs(dby) < 8) current_corner = BottomRight;
    //if(qAbs(dlx) < 8 && qAbs(dby) < 8) current_corner = BottomLeft;


    /*switch(current_corner)
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
    default:
        qDebug() << "None";
        this->setCursor(Qt::ArrowCursor);
        break;

    }*/

    QGraphicsItem::hoverMoveEvent(event);
}

//=========================================================================================================
void SvgDocument::attachGrabbers(const QRectF &rect)
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

void SvgDocument::updateCornersPosition()
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
