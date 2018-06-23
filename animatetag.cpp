#include "animatetag.h"
#include <QDebug>
#include <QGraphicsScene>
#include "rectitem.h"
#include "lineitem.h"
#include "ellipseitem.h"

int AnimateTag::ID = 0;

//=========================================================================================================
AnimateTag::AnimateTag(QObject *parent) : QPropertyAnimation(parent), currentID(ID++)
{
    name = QString::fromUtf8(propertyName()) + " " + name + " " + QString::number(currentID);
}

//=========================================================================================================
AnimateTag::AnimateTag(QObject *target, const QByteArray &propertyName, QObject *parent) : QPropertyAnimation(target, propertyName,parent), currentID(ID++)
{
    name = QString::fromUtf8(propertyName) + " " + name + " " + QString::number(currentID);
}

//=========================================================================================================
void AnimateTag::changedAttributeName(QString v)
{
    this->setPropertyName(v.toUtf8());
    qDebug() << "propertyName " << this->propertyName();
}

//=========================================================================================================
void AnimateTag::changedDuration(int v)
{
    this->setDuration(v);
    qDebug() << "duration " << this->duration();
}

//=========================================================================================================
void AnimateTag::changedRepeatCount(int v)
{
    this->setLoopCount(v);
    qDebug() << "loopCount " << this->loopCount();
}

//=========================================================================================================
void AnimateTag::changedFrom(QVariant v)
{
    this->setStartValue(v);
    qDebug() << "startValue " << this->startValue();
}

//=========================================================================================================
void AnimateTag::changedTo(QVariant v)
{
    this->setEndValue(v);
    qDebug() << "endValue " << this->endValue();
}

//=========================================================================================================
void AnimateTag::startSlot()
{
    this->start();
}

//=========================================================================================================
void AnimateTag::stopSlot()
{
    this->stop();
}

//=========================================================================================================
void AnimateTag::startAnimationOnCopy(QGraphicsItem *i, bool with_destroying)
{
    {
        RectItem* item = dynamic_cast<RectItem*>(i);
        if(item != nullptr)
        {
            if(with_destroying)
                connect(this, SIGNAL(finished()), this, SLOT(restoreTargetObjectWithDestroy()));
            else
                connect(this, SIGNAL(finished()), this, SLOT(restoreTargetObject()));
            origin_target = dynamic_cast<QGraphicsItem*>(this->targetObject());
            this->setTargetObject(item);
            this->start();
            qDebug() << "ANIMATION STARTED";
        }
    }
}

//=========================================================================================================
void AnimateTag::restoreTargetObject()
{
    if(origin_target != nullptr)
    {
        RectItem* item = dynamic_cast<RectItem*>(origin_target);
        if(item != 0)
        {
            disconnect(this, SIGNAL(finished()), this, SLOT(restoreTargetObject()));
            auto temp_target = dynamic_cast<RectItem*>(this->targetObject());
            this->setTargetObject(item);
            qDebug() << "ORIGIN RESTORED";
        }

        origin_target = nullptr;
    }
}

void AnimateTag::restoreTargetObjectWithDestroy()
{
    if(origin_target != nullptr)
    {
        RectItem* item = dynamic_cast<RectItem*>(origin_target);
        if(item != 0)
        {
            disconnect(this, SIGNAL(finished()), this, SLOT(restoreTargetObjectWithDestroy()));
            auto temp_target = dynamic_cast<RectItem*>(this->targetObject());
            auto scene = origin_target->scene();
            scene->removeItem(temp_target);
            this->setTargetObject(item);
            qDebug() << "ORIGIN RESTORED WITH DESTROY";
        }

        origin_target = nullptr;
    }
}

