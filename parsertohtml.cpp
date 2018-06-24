#include "parsertohtml.h"
#include <QDebug>

ParserToHtml::ParserToHtml(EditorScene* scene, QString file_name)
{
    this->scene = scene;
    this->file_name = file_name;
    generateFile();
}

QString ParserToHtml::getText()
{
    return html_text;
}

void ParserToHtml::generateFile()
{
    html_text = "<!DOCTYPE html>"
                "<html lang=\"en\">"
                    "<head>"
                        "<meta charset=\"utf-8\">"
                        "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">"
                        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                        "<title>" + file_name + "</title>"
                    "</head>"
                    "<body>"
                        "<svg width=\"" + QString::number(scene->width()) + "\" height=\"" + QString::number(scene->height()) + "\">";
    /////////////////////////////////////////



    auto shapes = scene->items();
    for(auto item : shapes)
    {
        bool flag = true;
        if(flag){
            RectItem* new_item = dynamic_cast<RectItem*>(item);
            if(new_item != nullptr)
            {
                html_text.append(parseRectItem(new_item));
                flag = false;
            }
        }

        if(flag){
            EllipseItem* new_item = dynamic_cast<EllipseItem*>(item);
            if(new_item != nullptr)
            {
                html_text.append(parseEllipseItem(new_item));
                flag = false;
            }
        }

        if(flag){
            LineItem* new_item = dynamic_cast<LineItem*>(item);
            if(new_item != nullptr)
            {
                html_text.append(parseLineItem(new_item));
                flag = false;
            }
        }
    }



    /////////////////////////////////////////
    html_text.append(   "<\svg>"
                    "<\body>"
                        "<\html>");
}

QString ParserToHtml::parseRectItem(RectItem *item)
{
    QString text = "<rect ";
    text.append("x=\"" + QString::number(item->getX()) +"\" ");
    text.append("y=\"" + QString::number(item->getY()) +"\" ");
    text.append("rx=\"" + QString::number(item->getRX()) +"\" ");
    text.append("ry=\"" + QString::number(item->getRY()) +"\" ");
    text.append("width=\"" + QString::number(item->getW()) +"\" ");
    text.append("height=\"" + QString::number(item->getH()) +"\" ");
    text.append("stroke=\"" + item->getStrokeColor().name() +"\" ");
    text.append("stroke-width=\"" + QString::number(item->getStrokeWidth()) +"\" ");
    text.append("stroke-opacity=\"" + QString::number(item->getStrokeColor().alphaF()) +"\" ");
    text.append("fill=\"" + item->getFillColor().name() +"\" ");
    text.append("fill-opacity=\"" + QString::number(item->getFillColor().alphaF()) +"\" ");
    text.append("z=\"" + QString::number(item->zValue()) +"\"");
    text.append(">");

    for(auto a : item->getAnimations())
        text.append(parseAnimation(a));

    text.append("</rect>");

    return text;
}

QString ParserToHtml::parseEllipseItem(EllipseItem *item)
{
    QString text = "<ellipse ";
    text.append("cx=\"" + QString::number(item->getX() + item->getW()/2) +"\" ");
    text.append("cy=\"" + QString::number(item->getY() + item->getH()/2) +"\" ");
    text.append("rx=\"" + QString::number(item->getW()/2) +"\" ");
    text.append("ry=\"" + QString::number(item->getH()/2) +"\" ");
    text.append("stroke=\"" + item->getStrokeColor().name() +"\" ");
    text.append("stroke-width=\"" + QString::number(item->getStrokeWidth()) +"\" ");
    text.append("stroke-opacity=\"" + QString::number(item->getStrokeColor().alphaF()) +"\" ");
    text.append("fill=\"" + item->getFillColor().name() +"\" ");
    text.append("fill-opacity=\"" + QString::number(item->getFillColor().alphaF()) +"\" ");
    text.append("z=\"" + QString::number(item->zValue()) +"\"");
    text.append(">");

    for(auto a : item->getAnimations())
        text.append(parseAnimation(a));

    text.append("</ellipse>");

    return text;
}

QString ParserToHtml::parseLineItem(LineItem *item)
{
    QString text = "<line ";
    text.append("x1=\"" + QString::number(item->getX1()) +"\" ");
    text.append("y1=\"" + QString::number(item->getY1()) +"\" ");
    text.append("x2=\"" + QString::number(item->getX2()) +"\" ");
    text.append("y2=\"" + QString::number(item->getY2()) +"\" ");
    text.append("stroke=\"" + item->getStrokeColor().name() +"\" ");
    text.append("stroke-width=\"" + QString::number(item->getStrokeWidth()) +"\" ");
    text.append("stroke-opacity=\"" + QString::number(item->getStrokeColor().alphaF()) +"\" ");
    text.append("z=\"" + QString::number(item->zValue()) +"\"");
    text.append(">");

    for(auto a : item->getAnimations())
        text.append(parseAnimation(a));

    text.append("</line>");

    return text;
}

QString ParserToHtml::parseAnimation(AnimateTag *animation)
{
    QString text = "<animate ";
    text.append("attributeName=\"" + QString::fromUtf8(animation->propertyName()) +"\" ");

    if(QString::fromUtf8(animation->propertyName()) == "strokeColor" || QString::fromUtf8(animation->propertyName()) == "fillColor")
        text.append("from=\"" + animation->startValue().value<QColor>().name() +"\" ");
    else
        text.append("from=\"" + QString::number(animation->startValue().toInt()) +"\" ");

    if(QString::fromUtf8(animation->propertyName()) == "strokeColor" || QString::fromUtf8(animation->propertyName()) == "fillColor")
        text.append("to=\"" + animation->endValue().value<QColor>().name() +"\" ");
    else
        text.append("to=\"" + QString::number(animation->endValue().toInt()) +"\" ");

    text.append("dur=\"" + QString::number(animation->duration()) +"ms\" ");
    text.append("repeatCount=\"" + QString::number(animation->loopCount()) +"\" ");

    text.append(">");

    return text;
}
