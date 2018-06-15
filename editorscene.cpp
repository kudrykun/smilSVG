#include "editorscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>
#include "document.h"
#include "rectitem.h"
#include "ellipseitem.h"
#include "grouppingitem.h"


//=========================================================================================================
EditorScene::EditorScene(QObject *parent) : QGraphicsScene(parent)
{
    currentMode = Rect;
    currentKeyMode = None;
    current_scale = 1;
    setSceneRect(0,0,2000,2000);

    document = new SvgDocument(QRectF(350,350,1000,1000));
    connect(this, SIGNAL(updateDocumentScale(qreal)), document, SLOT(setScaleFactor(qreal)));
    //RectItem *rect = new RectItem(QRectF(300,300,400,400));
    //EllipseItem *ellipse = new EllipseItem(QRectF(700,300,500,500));

    this->addItem(document);
    //this->addItem(rect);
    //this->addItem(ellipse);
    this->drawTestFigures();
}

//=========================================================================================================
void EditorScene::updateScale(qreal scale)
{
    current_scale *= scale;
    emit updateDocumentScale(scale);
}

void EditorScene::groupAction()
{
    qDebug() << "GROUP ACTION";
    qDebug() << this->selectedItems().size();
    GrouppingItem *group = new GrouppingItem();
    for(auto &e : this->selectedItems())
    {
        group->addToGroup(e);
    }
    this->addItem(group);
}

//=========================================================================================================
//переопределнный метод, используемый для отрисовки сетки
void EditorScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setPen(QPen(sceneColor));
    painter->setBrush(QBrush(sceneColor));
    painter->drawRect(this->sceneRect());
}


//=========================================================================================================
void EditorScene::drawTestFigures()
{
    for(int i = 0; i < 10; i++)
    {
        float y = 100;
        //прозрачность обводки
        {
            EllipseItem *ellipse = new EllipseItem(QRectF(10*i+i*80, y, 80, 80));
            ellipse->setStrokeWidth(10);
            ellipse->setStrokeOpacity(i/10.0);
            this->addItem(ellipse);
            y += 100;
        }

        //dasharray
        {
            EllipseItem *ellipse = new EllipseItem(QRectF(10*i+i*80, y, 80, 80));
            ellipse->setStrokeWidth(10);
            QVector<qreal> vec;
            vec.push_back(0.5/(i+1));
            vec.push_back(0.5*i);
            ellipse->setStrokeDasharray(vec);
            this->addItem(ellipse);
            y += 100;
        }


        //прозрачность обводки
        {
            EllipseItem *ellipse = new EllipseItem(QRectF(10*i+i*80, y, 80, 80));
            ellipse->setStrokeWidth(10);
            ellipse->setStrokeOpacity(i/10.0);
            this->addItem(ellipse);
            y += 100;
        }
    }



//    qsrand(100);
//    for(int i = 0; i < 10; i++)
//    {

//        EllipseItem *ellipse = new EllipseItem(QRectF(qrand() % 1000 + 200,qrand() % 1000 + 200,qrand() % 500 + 100,qrand() % 500 + 100));
//        //ellipse->setPen(QPen(QColor()));
//        ellipse->setBrush(QBrush(QColor(qrand() % 255,qrand() % 255,qrand() % 255)));
//        ellipse->setPen(QPen(QColor(qrand() % 255,qrand() % 255,qrand() % 255), qrand()%100));
//        this->addItem(ellipse);
    //    }
}
