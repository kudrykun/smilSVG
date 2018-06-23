#ifndef LINEITEM_H
#define LINEITEM_H


#include <QObject>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include "grabbingcorner.h"
#include <QGraphicsSceneMouseEvent>
#include "animatetag.h"
#include <QDebug>

class LineItem : public  QObject, public QGraphicsLineItem
{
    Q_OBJECT
    Q_PROPERTY(int x1 READ getX1 WRITE setX1 NOTIFY animationX1ChangedSignal)
    Q_PROPERTY(int y1 READ getY1 WRITE setY1 NOTIFY animationY1ChangedSignal)
    Q_PROPERTY(int x2 READ getX2 WRITE setX2 NOTIFY animationX2ChangedSignal)
    Q_PROPERTY(int y2 READ getY2 WRITE setY2 NOTIFY animationY2ChangedSignal)
    Q_PROPERTY(QColor strokeColor READ getStrokeColor WRITE setStrokeColor NOTIFY animationStrokeColorChangedSignal)
    Q_PROPERTY(int strokeOpacity READ getStrokeOpacity WRITE setStrokeOpacity NOTIFY animationStrokeOpacityChangedSignal)
    Q_PROPERTY(int strokeWidth READ getStrokeWidth WRITE setStrokeWidth NOTIFY animationStrokeWidthChangedSignal)
public:
    enum Corners{
        TopLeft = 4,
        BottomRight = 7
    };

public:
    LineItem(const QLineF &line, bool animationItem = false, QGraphicsItem* parent = nullptr);
    LineItem* copy(bool animationItem = false);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setPen(const QPen &v) {currentPen = v;}
    QPen getPen();

    QList<AnimateTag*> getAnimations() {return animations;}
    void addAnimation(QString name);
    void deleteAnimation(AnimateTag* a);
    QStringList getAnimAttrNames() {return animAttributesNames;}

    //это нужно для свойств анимаций
    int getX1(){return line().p1().x();}
    void setX1(int v) {x1 = v;emit animationX1ChangedSignal(v); emit x1ChangedSignal(v); qDebug() << "setX1";}

    int getY1(){return line().p1().y();}
    void setY1(int v){y1 = v;emit animationY1ChangedSignal(v); emit y1ChangedSignal(v); qDebug() << "setY1";}

    int getX2(){return line().p2().x();}
    void setX2(int v) {x1 = v;emit animationX1ChangedSignal(v); emit x1ChangedSignal(v); qDebug() << "setX1";}

    int getY2(){return line().p2().y();}
    void setY2(int v){y2 = v;emit animationY1ChangedSignal(v); emit y2ChangedSignal(v); qDebug() << "setY1";}

    QColor getStrokeColor(){return strokeColor;}
    void setStrokeColor(const QColor &v){strokeColor = v;emit animationStrokeColorChangedSignal(v); emit strokeColorChangedSignal(v); qDebug() << "setSTROKECOLOR";}

    int getStrokeOpacity(){return strokeOpacity;}
    void setStrokeOpacity(int v){strokeOpacity = v;emit animationStrokeOpacityChangedSignal(v); emit strokeOpacityChangedSignal(v); qDebug() << "setX";}

    int getStrokeWidth(){return strokeWidth;}
    void setStrokeWidth(int v){strokeWidth = v;emit animationStrokeWidthChangedSignal(v); emit strokeWidthChangedSignal(v); qDebug() << "setSTROKE WIDTH";}



signals:
    //animation signals
    void animationX1ChangedSignal(int v);
    void animationY1ChangedSignal(int v);
    void animationX2ChangedSignal(int v);
    void animationY2ChangedSignal(int v);
    void animationStrokeColorChangedSignal(QColor v);
    void animationStrokeOpacityChangedSignal(int v);
    void animationStrokeWidthChangedSignal(int v);


    void x1ChangedSignal(int v);
    void y1ChangedSignal(int v);
    void x2ChangedSignal(int v);
    void y2ChangedSignal(int v);
    void strokeColorChangedSignal(QColor c);
    void strokeWidthChangedSignal(int w);
    void strokeOpacityChangedSignal(int v);

public slots:
    void setScaleFactor(qreal factor);
    void cornerMove(GrabbingCorner *owner, qreal dx, qreal dy);

    void x1Changed(int v);
    void y1Changed(int v);
    void x2Changed(int v);
    void y2Changed(int v);
    void strokeColorChanged(QColor c);
    void strokeWidthChanged(int w);

    void playAnimations();
    void stopAnimations();

    void startAnimation(AnimateTag* a);

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

    bool debug_mode = false;

    bool animation_item = true;
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    QColor strokeColor = QColor(120,120,120);
    int strokeOpacity = 255;
    int strokeWidth = 1;

    QList<AnimateTag*> animations;
    QStringList animAttributesNames;
};

#endif // LINEITEM_H
