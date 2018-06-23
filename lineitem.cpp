#include "lineitem.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QCursor>
#include <QGraphicsScene>

//=========================================================================================================
LineItem::LineItem(const QLineF &line, bool animationItem, QGraphicsItem* parent) : QGraphicsLineItem(line, parent)
{
    setLine(line);
    setPen(currentPen);
    if(!animationItem)
        setFlags(ItemIsSelectable|ItemSendsGeometryChanges);
    else
        setFlags(ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    current_corner = 0;

    connect(this, SIGNAL(animationX1ChangedSignal(int)),this, SLOT(x1Changed(int)));
    connect(this, SIGNAL(animationY1ChangedSignal(int)),this, SLOT(y1Changed(int)));
    connect(this, SIGNAL(animationX2ChangedSignal(int)),this, SLOT(x2Changed(int)));
    connect(this, SIGNAL(animationY2ChangedSignal(int)),this, SLOT(y2Changed(int)));
    connect(this, SIGNAL(animationStrokeColorChangedSignal(QColor)),this, SLOT(strokeColorChanged(QColor)));
    connect(this, SIGNAL(animationStrokeWidthChangedSignal(int)),this, SLOT(strokeWidthChanged(int)));

    //инициализация списка доступныз для анимации свойств
    {
        animAttributesNames.push_back("x1");
        animAttributesNames.push_back("y1");
        animAttributesNames.push_back("x2");
        animAttributesNames.push_back("y2");
        animAttributesNames.push_back("strokeColor");
        animAttributesNames.push_back("strokeWidth");
    }

    qDebug() << "LINE ITEM CREATED";
}

//=========================================================================================================
LineItem *LineItem::copy(bool animationItem)
{
    LineItem *newItem = new LineItem(this->line(), animationItem);
    newItem->setPen(this->getPen());
    newItem->setPos(this->pos());

    return newItem;
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
QPen LineItem::getPen()
{
    return currentPen;
}

//=========================================================================================================
void LineItem::addAnimation(QString name)
{
    AnimateTag *animation = new AnimateTag(this, name.toUtf8());
    animation->setDuration(1000);
    animation->setLoopCount(1);
    if(name == "strokeColor" || name == "fillColor")
    {
        animation->setStartValue(QColor(255,255,255));
        animation->setEndValue(QColor(255,255,255));
    }
    else
    {
        animation->setStartValue(100);
        animation->setEndValue(1000);
    }

    animations.push_back(animation);
}

//=========================================================================================================
void LineItem::deleteAnimation(AnimateTag *a)
{
    animations.removeOne(a);
}

//=========================================================================================================
void LineItem::playAnimations()
{
    //короче, я создаю копию, на котой вопсроизводтся анимации, но копию удалаяютолько после завершения самой долгой анимации
    int max = 0;
    int max_index = 0;
    for(int i = 0; i < animations.size(); i++)
    {
        auto a = animations.at(i);
        if(max <= a->duration()*a->loopCount()){
            max = a->duration()*a->loopCount();
            max_index = i;
        }
    }

    auto new_rect = this->copy(true);

    //делаем полупрозрачной анимацию
    auto pen = new_rect->getPen();
    auto pen_color = pen.color();
    pen_color.setAlpha(pen_color.alpha()/2);
    pen.setColor(pen_color);
    new_rect->setPen(pen);

    this->scene()->addItem(new_rect);

    for(int i = 0; i < animations.size(); i++)
    {
        if(i == max_index)
            animations.at(i)->startAnimationOnCopy(new_rect, true);
        else
            animations.at(i)->startAnimationOnCopy(new_rect, false);
    }
}

//=========================================================================================================
void LineItem::stopAnimations()
{
    for(auto &a : animations)
        a->stopSlot();
}

//=========================================================================================================
void LineItem::startAnimation(AnimateTag *a)
{
    auto new_rect = this->copy(true);

    //делаем полупрозрачной анимацию
    auto pen = new_rect->getPen();
    auto pen_color = pen.color();
    pen_color.setAlpha(pen_color.alpha()/2);
    pen.setColor(pen_color);
    new_rect->setPen(pen);

    this->scene()->addItem(new_rect);
    a->startAnimationOnCopy(new_rect, true);
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
    if(!animation_item)
        updateCornersPosition();

    emit x1ChangedSignal(line.p1().x());
    emit y1ChangedSignal(line.p1().y());
    emit x2ChangedSignal(line.p2().x());
    emit y2ChangedSignal(line.p2().y());
}

//=========================================================================================================
void LineItem::x1Changed(int v)
{
    auto line = this->line();
    line.setP1(QPointF(v,line.p1().y()));
    this->setLine(line);
    update();
    if(!animation_item)
        updateCornersPosition();

    qDebug() << "LINE_CHANGED";
}

//=========================================================================================================
void LineItem::y1Changed(int v)
{
    auto line = this->line();
    line.setP1(QPointF(line.p1().x(),v));
    this->setLine(line);
    update();
    if(!animation_item)
        updateCornersPosition();

    qDebug() << "LINE_CHANGED";
}

//=========================================================================================================
void LineItem::x2Changed(int v)
{
    auto line = this->line();
    line.setP2(QPointF(v,line.p2().y()));
    this->setLine(line);
    update();
    if(!animation_item)
        updateCornersPosition();

    qDebug() << "LINE_CHANGED";
}

//=========================================================================================================
void LineItem::y2Changed(int v)
{
    auto line = this->line();
    line.setP2(QPointF(line.p2().x(),v));
    this->setLine(line);
    update();
    if(!animation_item)
        updateCornersPosition();

    qDebug() << "LINE_CHANGED";
}

//=========================================================================================================
void LineItem::strokeColorChanged(QColor c)
{
    currentPen.setColor(c);
    update();
}

//=========================================================================================================
void LineItem::strokeWidthChanged(int w)
{
    currentPen.setWidth(w);
    update();
    if(!animation_item)
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
        previous_pos = ev->pos();
        qDebug() << d << " " << this->line();
        //this->moveBy(d.x(), d.y());
        auto line = this->line();
        line.setP1(line.p1()+d);
        line.setP2(line.p2()+d);
        this->setLine(line);

        emit x1ChangedSignal(line.p1().x());
        emit y1ChangedSignal(line.p1().y());
        emit x2ChangedSignal(line.p2().x());
        emit y2ChangedSignal(line.p2().y());

        updateCornersPosition();
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

