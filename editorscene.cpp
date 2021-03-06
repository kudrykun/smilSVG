#include "editorscene.h"
#include "mainwindow.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>
#include "document.h"
#include "rectitem.h"
#include "ellipseitem.h"
#include "grouppingitem.h"
#include "lineitem.h"
#include <QGraphicsItemGroup>
#include <QPropertyAnimation>


//=========================================================================================================
EditorScene::EditorScene(QObject *parent) : QGraphicsScene(parent)
{
    currentMode = Rect;
    currentKeyMode = None;
    current_scale = 1;
    setSceneRect(0,0,2000,2000);

    connect(this, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));

    document = new SvgDocument(QRectF(350,350,1000,1000));
    connect(this, SIGNAL(updateDocumentScale(qreal)), document, SLOT(setScaleFactor(qreal)));
    RectItem *rect = new RectItem(QRectF(0,0,400,400));
    rect->setPos(300,300);
    EllipseItem *ellipse = new EllipseItem(QRectF(0,0,500,500));
    LineItem *line = new LineItem(QLineF(0,0,1100,700));
    line->setPos(QPointF(0,0));

    this->addItem(document);
    this->addItem(rect);
    this->addItem(ellipse);
    this->addItem(line);

//    QPropertyAnimation *animation = new QPropertyAnimation(rect, "x");
//    animation->setDuration(1000);
//    animation->setLoopCount(10);
//    animation->setStartValue(0);
//    animation->setEndValue(1000);

//    animation->start();

//    QPropertyAnimation *animation1 = new QPropertyAnimation(rect, "y");
//    animation1->setDuration(1000);
//    animation1->setLoopCount(10);
//    animation1->setStartValue(0);
//    animation1->setEndValue(1000);

//    animation1->start();

//    QPropertyAnimation *animation2 = new QPropertyAnimation(rect, "fillColor");
//    animation2->setDuration(1000);
//    animation2->setLoopCount(10);
//    animation2->setStartValue(QColor(Qt::red));
//    animation2->setEndValue(QColor(Qt::green));

//    animation2->start();

//    QPropertyAnimation *animation3 = new QPropertyAnimation(rect, "strokeColor");
//    animation3->setDuration(1000);
//    animation3->setLoopCount(10);
//    animation3->setStartValue(QColor(Qt::blue));
//    animation3->setEndValue(QColor(Qt::yellow));

//    animation3->start();

//    QPropertyAnimation *animation4 = new QPropertyAnimation(rect, "strokeWidth");
//    animation4->setDuration(1000);
//    animation4->setLoopCount(10);
//    animation4->setStartValue(1);
//    animation4->setEndValue(30);

//    animation4->start();


//    QPropertyAnimation *animation5 = new QPropertyAnimation(rect, "w");
//    animation5->setDuration(1000);
//    animation5->setLoopCount(10);
//    animation5->setStartValue(300);
//    animation5->setEndValue(600);

//    animation5->start();


//    QPropertyAnimation *animation6 = new QPropertyAnimation(rect, "h");
//    animation6->setDuration(1000);
//    animation6->setLoopCount(10);
//    animation6->setStartValue(400);
//    animation6->setEndValue(200);

//    animation6->start();

    //this->drawTestFigures();
}

//=========================================================================================================
void EditorScene::updateScale(qreal scale)
{
    current_scale *= scale;
    emit updateDocumentScale(scale);
}

//=========================================================================================================
void EditorScene::groupAction()
{
    GrouppingItem *group = new GrouppingItem();
    this->addItem(group);
    if(this->selectedItems().size() > 1)
    {
        for(auto &e : this->selectedItems())
        {
            group->addToGroup(e);
            e->setSelected(false);
        }
        group->setSelected(true);
    }

}

//=========================================================================================================
void EditorScene::ungroupAction()
{
    for(auto &e : this->selectedItems())
    {
        GrouppingItem* grouppingItem = qgraphicsitem_cast<GrouppingItem*>(e);
        if(grouppingItem != 0)
            this->destroyItemGroup(grouppingItem);
    }
}

//=========================================================================================================
void EditorScene::onSelectionChanged()
{
    auto items = this->selectedItems();
    if(items.size() == 1){
        bool flag = false;
        if(!flag){
            RectItem* new_item = dynamic_cast<RectItem*>(items.at(0));
            if(new_item != 0)
            {
                MainWindow* main_w = dynamic_cast<MainWindow*>(this->parent());
                if(main_w != nullptr)
                    main_w->createRightSideBar(new_item);
            }
        }

        if(!flag){
            EllipseItem* new_item = dynamic_cast<EllipseItem*>(items.at(0));
            if(new_item != 0)
            {
                MainWindow* main_w = dynamic_cast<MainWindow*>(this->parent());
                if(main_w != nullptr)
                    main_w->createRightSideBar(new_item);
            }
        }

        if(!flag){
            LineItem* new_item = dynamic_cast<LineItem*>(items.at(0));
            if(new_item != 0)
            {
                MainWindow* main_w = dynamic_cast<MainWindow*>(this->parent());
                if(main_w != nullptr)
                    main_w->createRightSideBar(new_item);
            }
        }
    }
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

}
