#ifndef ELLIPSEITEM_H
#define ELLIPSEITEM_H


#include <QObject>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include "grabbingcorner.h"
#include <QGraphicsSceneMouseEvent>

/*
 * Этот класс реализует площадь для манипуляцией свг элементами, который будет транслирован в xml разметку
*/
class EllipseItem : public  QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    EllipseItem(const QRectF &rect);
    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

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

public slots:
    void setScaleFactor(qreal factor);
    void cornerMove(GrabbingCorner *owner, qreal dx, qreal dy);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *ev) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    void drawGripCorners(QPainter *painter, const QRectF &rect);
    void attachGrabbers(const QRectF &rect);
    void removeGrabbers();
    void updateCornersPosition();
    QVariant itemChange(GraphicsItemChange change, const QVariant & value) override;

private:
    QBrush docBrush = QBrush(QColor(255,255,255));
    QPen docPen = QPen(QColor(160,160,160));

    bool selected = true;
    bool space_pressed = false;
    float cornerRad = 4;
    GrabbingCorner* grabbingCorners[8];
    unsigned int current_corner;
    QPointF previous_pos;
};

#endif // ELLIPSEITEM_H
