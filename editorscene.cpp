#include "editorscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>
#include "document.h"
#include "rectitem.h"
#include "ellipseitem.h"



//=========================================================================================================
EditorScene::EditorScene(QObject *parent) : QGraphicsScene(parent)
{
    currentMode = Rect;
    currentKeyMode = None;
    current_scale = 1;
    setSceneRect(0,0,2000,2000);

    document = new SvgDocument(QRectF(650,650,700,700));

    RectItem *rect = new RectItem(QRectF(300,300,400,400));
    EllipseItem *ellipse = new EllipseItem(QRectF(700,300,400,400));

    connect(this, SIGNAL(updateDocumentScale(qreal)), document, SLOT(setScaleFactor(qreal)));

    this->addItem(document);
    this->addItem(rect);
    this->addItem(ellipse);
}

//=========================================================================================================
void EditorScene::updateScale(qreal scale)
{
    current_scale *= scale;
    emit updateDocumentScale(scale);
}

//=========================================================================================================
//переопределнный метод, используемый для отрисовки сетки
void EditorScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setPen(QPen(sceneColor));
    painter->setBrush(QBrush(sceneColor));
    painter->drawRect(this->sceneRect());
}
