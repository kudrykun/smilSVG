#include "animatetag.h"
#include <QDebug>

int AnimateTag::ID = 0;

//=========================================================================================================
AnimateTag::AnimateTag(QObject *parent) : QPropertyAnimation(parent), currentID(ID++)
{
    name = QString::fromUtf8(propertyName()) + " " + name + " " + QString::number(currentID);
}

//=========================================================================================================
AnimateTag::AnimateTag(QObject *target, const QByteArray &propertyName, QObject *parent) : QPropertyAnimation(target, propertyName,parent), currentID(ID++)
{
    name = QString::fromUtf8(propertyName) + " " +name + " " + QString::number(currentID);
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

void AnimateTag::startSlot()
{
    this->start();
}

void AnimateTag::stopSlot()
{
    this->stop();
}
