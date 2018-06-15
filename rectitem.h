#ifndef RECTITEM_H
#define RECTITEM_H


#include <QObject>
#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>
#include "grabbingcorner.h"
#include <QGraphicsSceneMouseEvent>

class RectItem : public  QObject, public QGraphicsRectItem
{
    Q_OBJECT
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
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    QPen getPen();
    QBrush getBrush();
    void setStrokeWidth(float w);
    void setStrokeOpacity(float op);
    void setStrokeColor(QColor stroke_color);
    void setStrokeLineCap(Qt::PenCapStyle capStyle);
    void setStrokeLineJoin(Qt::PenJoinStyle joinStyle);
    void setStrokeDashoffset(qreal offset);
    void setStrokeDasharray(const QVector<qreal> & pattern);

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

    bool debug_mode = true;

    float rx = 40;
    float ry = 20;
};

#endif // RECTITEM_H
