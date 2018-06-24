#ifndef PARSERTOHTML_H
#define PARSERTOHTML_H

#include "editorscene.h"
#include <QGraphicsItem>
#include "rectitem.h"
#include "ellipseitem.h"
#include "lineitem.h"
#include "animatetag.h"

class ParserToHtml
{
public:
    ParserToHtml(EditorScene* scene, QString file_name = "New File");
    QString getText();
private:
    void generateFile();

    QString parseRectItem(RectItem* item);
    QString parseEllipseItem(EllipseItem* item);
    QString parseLineItem(LineItem* item);
    QString parseAnimation(AnimateTag* animation);

private:
    EditorScene* scene = nullptr;
    QString file_name;
    QString html_text;
};

#endif // PARSERTOHTML_H
