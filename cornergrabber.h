#ifndef CORNERGRABBER_H
#define CORNERGRABBER_H

#include <QObject>
#include <QGraphicsEllipseItem>

class CornerGrabber : public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    //CornerGrabber(QGraphicsItem *parentItem = 0, QObject *parent = 0);
    CornerGrabber(int x, int y, QGraphicsItem *parentItem = 0, QObject *parent = 0);
private:
    float cornerRadius = 8;
    QColor cornerColor = QColor(21,146,230);
};

#endif // CORNERGRABBER_H
