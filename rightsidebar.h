#ifndef RIGHTSIDEBAR_H
#define RIGHTSIDEBAR_H

#include <QToolBar>
#include <QGroupBox>
#include <QGraphicsItem>
#include "rectitem.h"
#include "ellipseitem.h"
#include "lineitem.h"

class RightSideBar : public QToolBar
{
    Q_OBJECT
public:
    enum ShowMode
    {
        None,
        Disabled,
        Common,
        Line,
        Ellipse,
        Rect
    };

    RightSideBar(QGraphicsItem *item = nullptr, QWidget *parent = nullptr);
    void setShowMode(ShowMode m) {showMode = m;}
    ShowMode getShowMode() {return showMode;}

signals:
    void setStrokeOpacitySignal(int v);
    void setFillOpacitySignal(int v);

public slots:
    void itemStrokeColorEdited();
    void itemFillColorEdited();

    void itemStrokeOpacityEdited(int v);
    void itemFillOpacityEdited(int v);
    void itemStrokeWEdited(int v);
    void itemXEdited(int v);
    void itemYEdited(int v);
    void itemX1Edited(int v);
    void itemY1Edited(int v);
    void itemX2Edited(int v);
    void itemY2Edited(int v);
    void itemRXEdited(int v);
    void itemRYEdited(int v);
    void itemWEdited(int v);
    void itemHEdited(int v);

private:
    void setupPositionPropBox(ShowMode mode);
    void setupAppearencePropBox(ShowMode mode);

private:
    RectItem *rect_item = nullptr;
    EllipseItem *ellipse_item = nullptr;
    LineItem *line_item = nullptr;

    ShowMode showMode;
    QGroupBox *positionProp = nullptr;
    QAction *positionPropTemp = nullptr;
    QGroupBox *appearenceProp = nullptr;
    QAction *appearencePropTemp = nullptr;

    int item_x = -1;
    int item_y = -1;
    int item_x1 = -1;
    int item_y1 = -1;
    int item_x2 = -1;
    int item_y2 = -1;
    int item_rx = -1;
    int item_ry = -1;
    int item_w = -1;
    int item_h = -1;
    int item_stroke_w = -1;
    QColor item_stroke_color = QColor(255,0,0);
    QColor item_fill_color = QColor(255,0,0);

};

#endif // RIGHTSIDEBAR_H
