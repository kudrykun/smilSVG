#ifndef LINEITEM_H
#define LINEITEM_H


#include <QObject>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include "grabbingcorner.h"
#include <QGraphicsSceneMouseEvent>

class LineItem : public  QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    enum Corners{
        TopLeft = 4,
        BottomRight = 7
    };

public:
    LineItem(const QLineF &line);
    LineItem* copy();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setPen(const QPen &pen);
    QPen getPen();
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
    QPen currentPen = QPen(QColor(120,120,120), 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);

    float cornerRad = 4;
    GrabbingCorner* grabbingCorners[2];
    unsigned int current_corner;
    QPointF previous_pos;

    bool debug_mode = true;
};

#endif // LINEITEM_H
