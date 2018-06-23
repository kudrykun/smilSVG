#include "rectitem.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QCursor>
#include "editorscene.h"

//=========================================================================================================
RectItem::RectItem(const QRectF &rect) : QGraphicsRectItem(rect)
{
    setRect(rect);
    setPen(currentPen);
    setBrush(currentBrush);
    setFlags(ItemIsSelectable|ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    current_corner = 0;
    connect(this, SIGNAL(animationXChangedSignal(int)),this, SLOT(xChanged(int)));
    connect(this, SIGNAL(animationYChangedSignal(int)),this, SLOT(yChanged(int)));
    connect(this, SIGNAL(animationWChangedSignal(int)),this, SLOT(wChanged(int)));
    connect(this, SIGNAL(animationHChangedSignal(int)),this, SLOT(hChanged(int)));
    connect(this, SIGNAL(animationRXChangedSignal(int)),this, SLOT(rxChanged(int)));
    connect(this, SIGNAL(animationRYChangedSignal(int)),this, SLOT(ryChanged(int)));
    connect(this, SIGNAL(animationStrokeColorChangedSignal(QColor)),this, SLOT(strokeColorChanged(QColor)));
    connect(this, SIGNAL(animationFillColorChangedSignal(QColor)),this, SLOT(fillColorChanged(QColor)));
    //connect(this, SIGNAL(animationStrokeOpacityChangedSignal(int)),this, SLOT(strokeColorChanged(QColor)));
    //connect(this, SIGNAL(animationFillOpacityChangedSignal(int)),this, SLOT(fillColorChanged(QColor)));
    connect(this, SIGNAL(animationStrokeWidthChangedSignal(int)),this, SLOT(strokeWidthChanged(int)));

//    for(int i = 0; i < 4; i++){

//        AnimateTag *animation = new AnimateTag(this, "x");
//        animation->setDuration(1000);
//        animation->setLoopCount(10);
//        animation->setStartValue(0);
//        animation->setEndValue(1000);

//        //animation->start();

//        animations.push_back(animation);
//    }

    //Задаем доступные attrname для этой фигуры
    {
        animAttributesNames.push_back("x");
        animAttributesNames.push_back("y");
        animAttributesNames.push_back("w");
        animAttributesNames.push_back("h");
        animAttributesNames.push_back("strokeColor");
        animAttributesNames.push_back("strokeOpacity");
        animAttributesNames.push_back("fillColor");
        animAttributesNames.push_back("fillOpacity");
        animAttributesNames.push_back("strokeWidth");
        animAttributesNames.push_back("rx");
        animAttributesNames.push_back("ry");
    }
}

//=========================================================================================================
RectItem *RectItem::copy()
{
    RectItem *newItem = new RectItem(this->rect());
    qDebug() << this->rect();
    newItem->setPen(this->getPen());
    newItem->setBrush(this->getBrush());
    newItem->setPos(this->pos());
    newItem->setItem_rx(item_rx);
    newItem->setItem_ry(item_ry);
    return newItem;
}

//=========================================================================================================
QRectF RectItem::boundingRect() const
{
    QRectF rect = this->rect();
    rect.adjust(-currentPen.width()/2,-currentPen.width()/2,currentPen.width()/2,currentPen.width()/2);
    if(isSelected()){
        rect.adjust(-cornerRad/2, - cornerRad/2, cornerRad/2, cornerRad/2);
    }
    return rect;
}

//=========================================================================================================
void RectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF re = this->rect();

    painter->setBrush(currentBrush);
    painter->setPen(currentPen);
    painter->drawRoundedRect(re,item_rx,item_ry);

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
QPen RectItem::getPen()
{
    return currentPen;
}

//=========================================================================================================
QBrush RectItem::getBrush()
{
    return currentBrush;
}

//=========================================================================================================
void RectItem::addAnimation(QString name)
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
void RectItem::deleteAnimation(AnimateTag *a)
{
    animations.removeOne(a);
}

//=========================================================================================================
void RectItem::playAnimations()
{
    for(auto &a : animations)
        a->start();
}

//=========================================================================================================
void RectItem::stopAnimations()
{
    for(auto &a : animations)
        a->stop();
}

void RectItem::startAnimation(AnimateTag *a)
{
    auto new_rect = this->copy();

    //делаем полупрозрачной анимацию
    auto pen = new_rect->getPen();
    auto pen_color = pen.color();
    pen_color.setAlpha(pen_color.alpha()/2);
    pen.setColor(pen_color);
    new_rect->setPen(pen);

    auto brush = new_rect->getBrush();
    auto brush_color = brush.color();
    brush_color.setAlpha(brush_color.alpha()/2);
    brush.setColor(brush_color);
    new_rect->setBrush(brush);

    this->scene()->addItem(new_rect);
    a->startAnimationOnCopy(new_rect);
}

//=========================================================================================================
void RectItem::setScaleFactor(qreal factor)
{
    cornerRad /= factor;
}

//=========================================================================================================
void RectItem::cornerMove(GrabbingCorner *owner, qreal dx, qreal dy)
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
    setRect(QRectF(0,0,tempRect.width(), tempRect.height()));
    setPos(QPointF(tempRect.x(), tempRect.y()) + pos());
    update();
    updateCornersPosition();

    emit wChangedSignal(tempRect.width());
    emit hChangedSignal(tempRect.height());
    emit xChangedSignal(pos().x());
    emit yChangedSignal(pos().y());
}

void RectItem::xChanged(int v)
{
    this->setPos(v, pos().y());
    x = v;
    qDebug() << "X_CHANGED!";
}

void RectItem::yChanged(int v)
{
    this->setPos(pos().x(), v);
}

void RectItem::wChanged(int v)
{
    auto rect = this->rect();
    qDebug() << "W_CHANGED";
    rect.setWidth(v);
    this->setRect(rect);
    update();
    if(!animation_item)
        updateCornersPosition();
}

void RectItem::hChanged(int v)
{
    auto rect = this->rect();
    qDebug() << rect;
    rect.setHeight(v);
    this->setRect(rect);
    update();
    if(!animation_item)
        updateCornersPosition();
}

void RectItem::strokeColorChanged(QColor c)
{
    currentPen.setColor(c);
    update();
}

void RectItem::fillColorChanged(QColor c)
{
    currentBrush.setColor(c);
    qDebug() << c;
    update();
}

void RectItem::strokeWidthChanged(int w)
{
    currentPen.setWidth(w);
    update();
    if(!animation_item)
        updateCornersPosition();
}

void RectItem::cornerRadChanged(int rx, int ry)
{
    this->item_rx = rx;
    this->item_ry = ry;
    update();
}

//=========================================================================================================
void RectItem::mousePressEvent(QGraphicsSceneMouseEvent *ev)
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
void RectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        this->setCursor(Qt::ArrowCursor);

            setSelected(true);
    }
}

//=========================================================================================================
void RectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
    if(isSelected()){
        auto d = ev->pos() - previous_pos;
        this->moveBy(d.x(), d.y());
        emit xChangedSignal(pos().x());
        emit yChangedSignal(pos().y());
        qDebug() << this->rect() << " " << this->pos();

    }
    QGraphicsItem::mouseMoveEvent(ev);
}

//=========================================================================================================
void RectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
}

//=========================================================================================================
void RectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent(event);
}

//=========================================================================================================
void RectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
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
void RectItem::attachGrabbers()
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
void RectItem::removeGrabbers()
{
    for(auto gc : grabbingCorners)
    {
       delete gc;
    }
}

//=========================================================================================================
void RectItem::updateCornersPosition()
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
QVariant RectItem::itemChange(RectItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == RectItem::ItemSelectedHasChanged)
    {
        if(isSelected())
            attachGrabbers();
        else
            removeGrabbers();
    }
    return QGraphicsItem::itemChange(change, value);
}

int RectItem::getItem_ry() const
{
    return item_ry;
}

void RectItem::setItem_ry(int value)
{
    item_ry = value;
}

int RectItem::getItem_rx() const
{
    return item_rx;
}

void RectItem::setItem_rx(int value)
{
    item_rx = value;
}
