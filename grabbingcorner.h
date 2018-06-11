#ifndef GRABBINGCORNER_H
#define GRABBINGCORNER_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>

class GrabbingCorner : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    enum CornerType{
        Top = 0,
        Bottom = 1,
        Left = 2,
        Right = 3,
        TopLeft = 4,
        TopRight = 5,
        BottomLeft = 6,
        BottomRight = 7
    };
    GrabbingCorner(QPointF pos, CornerType type, QGraphicsItem *paremtItem = nullptr, QObject *parent = nullptr);
    unsigned int getCornerType(){return corner_type;}
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *ev) override ;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

signals:
    void signalMove(GrabbingCorner *owner, qreal dx, qreal dy);

private:
    QPointF getPreviousPos() const;
    void setPreviousPos(const QPointF pos);
private:
    QColor strokeColor = QColor(21,146,230);
    QColor fillColor = QColor(255, 255, 255);
    QColor filHoverColor = QColor(21,146,230);
    qreal cornerRad = 8;
    QPointF previous_pos;
    unsigned int corner_type;
};

#endif // GRABBINGCORNER_H
