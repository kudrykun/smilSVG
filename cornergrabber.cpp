#include "cornergrabber.h"
#include <QBrush>
#include <QPen>
//=========================================================================================================
CornerGrabber::CornerGrabber(int x, int y, QGraphicsItem *parentItem, QObject *parent) : QGraphicsEllipseItem(parent)
{
    if(x >= 0 && x <=1 && y >= 0 && y <= 1)
    {
        setParentItem(parentItem);
        setBrush(QBrush(Qt::white));
        setPen(QPen(cornerColor));

        QRectF rect;
        rect.setWidth(cornerRadius);
        rect.setHeight(cornerRadius);

        if(x == 0.5)
            rect.setX(parentItem->boundingRect().width()*x);

        if(y == 0.5)
            rect.setY(parentItem->boundingRect().height()*y);

        if(y != 0.5 && x != 0.5){
            rect.setX(x * parentItem->boundingRect().width() + x == 0 ? -cornerRadius/2 : cornerRadius/2);
            rect.setY(y * parentItem->boundingRect().height() + y == 0 ? -cornerRadius/2 : cornerRadius/2);
        }
        setRect(rect);
    }


}
