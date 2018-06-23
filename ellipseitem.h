#ifndef ELLIPSEITEM_H
#define ELLIPSEITEM_H


#include <QObject>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include "grabbingcorner.h"
#include <QGraphicsSceneMouseEvent>
#include "animatetag.h"
#include <QDebug>

class EllipseItem : public  QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
    Q_PROPERTY(int x READ getX WRITE setX NOTIFY animationXChangedSignal)
    Q_PROPERTY(int y READ getY WRITE setY NOTIFY animationYChangedSignal)
    Q_PROPERTY(int w READ getW WRITE setW NOTIFY animationWChangedSignal)
    Q_PROPERTY(int h READ getH WRITE setH NOTIFY animationHChangedSignal)
    Q_PROPERTY(QColor strokeColor READ getStrokeColor WRITE setStrokeColor NOTIFY animationStrokeColorChangedSignal)
    Q_PROPERTY(QColor fillColor READ getFillColor WRITE setFillColor NOTIFY animationFillColorChangedSignal)
    Q_PROPERTY(int strokeOpacity READ getStrokeOpacity WRITE setStrokeOpacity NOTIFY animationStrokeOpacityChangedSignal)
    Q_PROPERTY(int fillOpacity READ getFillOpacity WRITE setFillOpacity NOTIFY animationFillOpacityChangedSignal)
    Q_PROPERTY(int strokeWidth READ getStrokeWidth WRITE setStrokeWidth NOTIFY animationStrokeWidthChangedSignal)
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
    EllipseItem(const QRectF &rect, bool animationItem = false);
    EllipseItem* copy(bool animationItem = false);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    QPen getPen();
    QBrush getBrush();

    QList<AnimateTag*> getAnimations() {return animations;}
    void addAnimation(QString name);
    void deleteAnimation(AnimateTag* a);
    QStringList getAnimAttrNames() {return animAttributesNames;}

    //это нужно для свойств анимаций
    int getX(){return pos().x();}
    void setX(int v) {x = v;emit animationXChangedSignal(v); emit xChangedSignal(v); qDebug() << "setX";}

    int getY(){return pos().y();}
    void setY(int v){y = v;emit animationYChangedSignal(v); emit yChangedSignal(v); qDebug() << "setY";}

    int getW(){return rect().width();}
    void setW(int v){w = v;emit animationWChangedSignal(v); emit wChangedSignal(v); qDebug() << "setW";}

    int getH(){return rect().height();}
    void setH(int v){h = v;emit animationHChangedSignal(v); emit hChangedSignal(v); qDebug() << "setH";}

    QColor getStrokeColor(){return strokeColor;}
    void setStrokeColor(const QColor &v){strokeColor = v;emit animationStrokeColorChangedSignal(v); emit strokeColorChangedSignal(v); qDebug() << "setSTROKECOLOR";}

    QColor getFillColor(){return fillColor;}
    void setFillColor(const QColor &v){fillColor = v;emit animationFillColorChangedSignal(v); emit fillColorChangedSignal(v); qDebug() << "setFILLCOLOR" << v;}

    int getStrokeOpacity(){return strokeOpacity;}
    void setStrokeOpacity(int v){strokeOpacity = v;emit animationStrokeOpacityChangedSignal(v); emit strokeOpacityChangedSignal(v); qDebug() << "setX";}

    int getFillOpacity(){return fillOpacity;}
    void setFillOpacity(int v){fillOpacity = v;emit animationFillOpacityChangedSignal(v); emit fillOpacityChangedSignal(v); qDebug() << "setX";}

    int getStrokeWidth(){return strokeWidth;}
    void setStrokeWidth(int v){strokeWidth = v;emit animationStrokeWidthChangedSignal(v); emit strokeWidthChangedSignal(v); qDebug() << "setSTROKE WIDTH";}


signals:
    //animation signals
    void animationXChangedSignal(int v);
    void animationYChangedSignal(int v);
    void animationWChangedSignal(int v);
    void animationHChangedSignal(int v);
    void animationRXChangedSignal(int v);
    void animationRYChangedSignal(int v);
    void animationStrokeColorChangedSignal(QColor v);
    void animationFillColorChangedSignal(QColor v);
    void animationStrokeOpacityChangedSignal(int v);
    void animationFillOpacityChangedSignal(int v);
    void animationStrokeWidthChangedSignal(int v);

    void xChangedSignal(int v);
    void yChangedSignal(int v);
    void wChangedSignal(int v);
    void hChangedSignal(int v);
    void strokeColorChangedSignal(QColor c);
    void fillColorChangedSignal(QColor c);
    void strokeWidthChangedSignal(int w);
    void strokeOpacityChangedSignal(int v);
    void fillOpacityChangedSignal(int v);

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
    QBrush currentBrush = QBrush(QColor(255,255,255));
    QPen currentPen = QPen(QColor(120,120,120), 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);

    float cornerRad = 4;
    GrabbingCorner* grabbingCorners[8];
    unsigned int current_corner;
    QPointF previous_pos;

    bool debug_mode = false;

    //то что относится к анимациям
    QStringList animAttributesNames;
    QList<AnimateTag*> animations;

    bool animation_item = true;
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    int rx = 0;
    int ry = 0;
    QColor strokeColor = QColor(120,120,120);
    QColor fillColor = QColor(255,255,255);
    int strokeOpacity = 255;
    int fillOpacity = 255;
    int strokeWidth = 1;

};

#endif // ELLIPSEITEM_H
