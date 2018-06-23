#ifndef ANIMATETAG_H
#define ANIMATETAG_H

#include <QObject>
#include <QPropertyAnimation>
#include <QGraphicsItem>

class AnimateTag : public QPropertyAnimation
{
    Q_OBJECT
public:
    AnimateTag(QObject * parent = 0);
    AnimateTag(QObject * target, const QByteArray & propertyName, QObject * parent = 0);
    QString getName() {return name;}
    void setName(QString n) {name = n;}

public slots:
    void changedAttributeName(QString v);
    void changedDuration(int v);
    void changedRepeatCount(int v);
    void changedFrom(QVariant v);
    void changedTo(QVariant v);

    //слотовые обертки для одноименных функций
    void startSlot();
    void stopSlot();

    void startAnimationOnCopy(QGraphicsItem* i, bool with_destroying = false); //задает новый временный объект для анимации и запускает ее
    void restoreTargetObject(); //восстанавливает исходный объект
    void restoreTargetObjectWithDestroy(); //восстанавливает исходный объект


private:
   static int ID;
   const int currentID;
   QString name = "Animation";
   QGraphicsItem *origin_target = nullptr;
//   RectItem *origin_rect = nullptr;
//   EllipseItem *origin_ellipse = nullptr;
//   LineItem *origin_line = nullptr;
};


#endif // ANIMATETAG_H
