#ifndef RECTITEM_H
#define RECTITEM_H


#include <QObject>
#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>
#include "grabbingcorner.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPropertyAnimation>
#include "animatetag.h"
#include <QStringList>

class RectItem : public  QObject, public QGraphicsRectItem
{
    Q_OBJECT
    Q_PROPERTY(int x READ getX WRITE setX NOTIFY animationXChangedSignal)
public:
    enum Corners{
        Top = 0,
        Bottom = 1,
        Left = 2,
        Right = 3,
        TopLeft = 4,
        TopRight = 5,
        BottomLeft = 6,
        BottomRight = 7
    };

public:
    RectItem(const QRectF &rect);
    RectItem* copy();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPen getPen();
    QBrush getBrush();
    float getRx() {return rx;}
    float getRy() {return ry;}

    QVector<AnimateTag*> getAnimations() {return animations;}
    void addAnimation();
    QStringList getAnimAttrNames() {return animAttributesNames;}

    //это нужно для свойств анимаций
    int getX(){return pos().x();}
    void setX(int v) {x = v;emit animationXChangedSignal(v); emit xChangedSignal(v); qDebug() << "setX";}

signals:
    void animationXChangedSignal(int v);

    void xChangedSignal(int v);
    void yChangedSignal(int v);
    void wChangedSignal(int v);
    void hChangedSignal(int v);
    void strokeColorChangeSignal(QColor c);
    void fillColorChangedSignal(QColor c);
    void strokeWidthChangedSignal(int w);
    void cornerRadChangedSignal(int rx, int ry);

public slots:
    void setScaleFactor(qreal factor);
    void cornerMove(GrabbingCorner *owner, qreal dx, qreal dy);

    void xChanged(int v);
    void yChanged(int v);
    void wChanged(int v);
    void hChanged(int v);
    void strokeColorChanged(QColor c);
    void fillColorChanged(QColor c);
    void strokeWidthChanged(int w);
    void cornerRadChanged(int rx, int ry);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *ev) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    void attachGrabbers();
    void removeGrabbers();
    void updateCornersPosition();
    QVariant itemChange(GraphicsItemChange change, const QVariant & value) override;

private:
    QBrush docBrush = QBrush(QColor(255,255,255));
    QBrush currentBrush = QBrush(QColor(255,255,255));
    QPen currentPen = QPen(QColor(120,120,120), 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);

    float cornerRad = 4;
    GrabbingCorner* grabbingCorners[8];
    unsigned int current_corner;
    QPointF previous_pos;

    bool debug_mode = false;

    float rx = 0;
    float ry = 0;
    int x = 0;

    QVector<AnimateTag*> animations;
    QStringList animAttributesNames;
};

#endif // RECTITEM_H
