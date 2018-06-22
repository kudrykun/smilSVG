#ifndef ELLIPSEITEM_H
#define ELLIPSEITEM_H


#include <QObject>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include "grabbingcorner.h"
#include <QGraphicsSceneMouseEvent>

class EllipseItem : public  QObject, public QGraphicsEllipseItem
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
    EllipseItem(const QRectF &rect);
    EllipseItem* copy();
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

signals:
    void xChangedSignal(int v);
    void yChangedSignal(int v);
    void wChangedSignal(int v);
    void hChangedSignal(int v);
    void strokeColorChangeSignal(QColor c);
    void fillColorChangedSignal(QColor c);
    void strokeWidthChangedSignal(int w);

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
    QBrush currentBrush = QBrush(QColor(255,255,255));
    QPen currentPen = QPen(QColor(120,120,120), 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);

    float cornerRad = 4;
    GrabbingCorner* grabbingCorners[8];
    unsigned int current_corner;
    QPointF previous_pos;

    bool debug_mode = false;
};

#endif // ELLIPSEITEM_H
