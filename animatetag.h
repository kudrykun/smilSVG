#ifndef ANIMATETAG_H
#define ANIMATETAG_H

#include <QObject>
#include <QPropertyAnimation>

class AnimateTag : public QPropertyAnimation
{
    Q_OBJECT
public:
    AnimateTag(QObject * parent = 0);
    AnimateTag(QObject * target, const QByteArray & propertyName, QObject * parent = 0);
    QString getName() {return name;}
    void setName(QString n) {name = n;}
    /*
     * AnimateTag *animation = new AnimateTag(this, "x");
        animation->setDuration(1000);
        animation->setLoopCount(10);
        animation->setStartValue(0);
        animation->setEndValue(1000);

        //animation->start();

        animations.push_back(animation);
*/

public slots:
    void changedAttributeName(QString v);
    void changedDuration(int v);
    void changedRepeatCount(int v);
    void changedFrom(QVariant v);
    void changedTo(QVariant v);

private:
   static int ID;
   const int currentID;
   QString name = "Animation";
};


#endif // ANIMATETAG_H
