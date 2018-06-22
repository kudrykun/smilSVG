#include "rightsidebar.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QIntValidator>
#include <QPushButton>
#include <QColorDialog>
#include <QDebug>

//=========================================================================================================
RightSideBar::RightSideBar(QGraphicsItem *item, QWidget *parent) : QToolBar(parent)
{
    showMode = Rect;

    if(item != nullptr){
        bool flag = false;
        if(!flag){
            RectItem* new_item = dynamic_cast<RectItem*>(item);
            if(new_item != 0)
            {
                qDebug() << "RECT";
                showMode = Rect;
                item_x = new_item->pos().x();
                item_y = new_item->pos().y();

                item_w = new_item->rect().width();
                item_h = new_item->rect().height();
                item_rx = new_item->getRx();
                item_ry = new_item->getRy();
                item_stroke_w = new_item->getPen().width();
                item_stroke_color = new_item->getPen().color();
                item_fill_color = new_item->getBrush().color();
                this->rect_item = new_item;
            }
        }

        if(!flag){
            EllipseItem* new_item = dynamic_cast<EllipseItem*>(item);
            if(new_item != 0)
            {
                qDebug() << "ELLIPSE";
                showMode = Ellipse;
                item_x = new_item->pos().x();
                item_y = new_item->pos().y();

                item_w = new_item->rect().width();
                item_h = new_item->rect().height();
                item_stroke_w = new_item->getPen().width();
                item_stroke_color = new_item->getPen().color();
                item_fill_color = new_item->getBrush().color();
                this->ellipse_item = new_item;
            }
        }

        if(!flag){
            LineItem* new_item = dynamic_cast<LineItem*>(item);
            if(new_item != 0)
            {
                qDebug() << "LINE";
                showMode = Line;
                item_x1 = new_item->line().p1().x();
                item_y1 = new_item->line().p1().y();
                item_x2 = new_item->line().p2().x();
                item_y2 = new_item->line().p2().y();
                item_stroke_w = new_item->getPen().width();
                item_stroke_color = new_item->getPen().color();
                this->line_item = new_item;
            }
        }
    }

    setupPositionPropBox(showMode);
    positionPropTemp = this->addWidget(positionProp);

    setupAppearencePropBox(showMode);
    appearencePropTemp = this->addWidget(appearenceProp);
}

//=========================================================================================================
void RightSideBar::itemStrokeColorEdited()
{
    QColorDialog::ColorDialogOptions options = QFlag(QColorDialog::ShowAlphaChannel);
    const QColor color = QColorDialog::getColor(item_stroke_color, this, "Select stroke color", options);
    if (color.isValid()) {
        item_stroke_color = color;

        setupAppearencePropBox(showMode);
        appearencePropTemp = this->addWidget(appearenceProp);

        emit setStrokeOpacitySignal(color.alpha());

        if(rect_item != nullptr)
            rect_item->strokeColorChanged(color);

        if(ellipse_item != nullptr)
            ellipse_item->strokeColorChanged(color);

        if(line_item != nullptr)
            line_item->strokeColorChanged(color);
    }
}

//=========================================================================================================
void RightSideBar::itemFillColorEdited()
{
    QColorDialog::ColorDialogOptions options = QFlag(QColorDialog::ShowAlphaChannel);
    const QColor color = QColorDialog::getColor(item_fill_color, this, "Select fill color",options);
    if (color.isValid()) {
        item_fill_color = color;

        setupAppearencePropBox(showMode);
        appearencePropTemp = this->addWidget(appearenceProp);

        emit setFillOpacitySignal(color.alpha());

        if(rect_item != nullptr)
            rect_item->fillColorChanged(color);

        if(ellipse_item != nullptr)
            ellipse_item->fillColorChanged(color);
    }
}

//=========================================================================================================
void RightSideBar::itemStrokeOpacityEdited(int v)
{
    item_stroke_color.setAlpha(v);
    if(rect_item != nullptr)
        rect_item->strokeColorChanged(item_stroke_color);
    if(ellipse_item != nullptr)
        ellipse_item->strokeColorChanged(item_stroke_color);
    if(line_item != nullptr)
        line_item->strokeColorChanged(item_stroke_color);
}

//=========================================================================================================
void RightSideBar::itemFillOpacityEdited(int v)
{
    item_fill_color.setAlpha(v);
    if(rect_item != nullptr)
        rect_item->fillColorChanged(item_fill_color);
    if(ellipse_item != nullptr)
        ellipse_item->fillColorChanged(item_fill_color);

}

//=========================================================================================================
void RightSideBar::itemStrokeWEdited(int v)
{
    item_stroke_w = v;

    if(rect_item != nullptr)
        rect_item->strokeWidthChanged(item_stroke_w);
    if(ellipse_item != nullptr)
        ellipse_item->strokeWidthChanged(item_stroke_w);
    if(line_item != nullptr)
        line_item->strokeWidthChanged(item_stroke_w);
}

//=========================================================================================================
void RightSideBar::itemXEdited(int op)
{
    item_x = op;
    QSpinBox* new_item = dynamic_cast<QSpinBox*>(QObject::sender());
    if(new_item->hasFocus()){
        if(rect_item != nullptr)
            rect_item->xChanged(item_x);

        if(ellipse_item != nullptr)
            ellipse_item->xChanged(item_x);
    }

}

//=========================================================================================================
void RightSideBar::itemYEdited(int op)
{
    item_y = op;
    QSpinBox* new_item = dynamic_cast<QSpinBox*>(QObject::sender());
    if(new_item->hasFocus()){
        if(rect_item != nullptr)
            rect_item->yChanged(item_y);
        if(ellipse_item != nullptr)
            ellipse_item->yChanged(item_y);
    }
}

void RightSideBar::itemX1Edited(int v)
{
    item_x1 = v;
    if(line_item != nullptr)
        line_item->x1Changed(item_x1);
}

void RightSideBar::itemY1Edited(int v)
{
    item_y1 = v;
    if(line_item != nullptr)
        line_item->y1Changed(item_y1);
}

void RightSideBar::itemX2Edited(int v)
{
    item_x2 = v;
    if(line_item != nullptr)
        line_item->x2Changed(item_x2);
}

void RightSideBar::itemY2Edited(int v)
{
    item_y2 = v;
    if(line_item != nullptr)
        line_item->y2Changed(item_y2);
}

void RightSideBar::itemRXEdited(int v)
{
    item_rx = v;

    if(rect_item != nullptr)
        rect_item->cornerRadChanged(item_rx,item_ry);
}

void RightSideBar::itemRYEdited(int v)
{
    item_ry = v;

    if(rect_item != nullptr)
        rect_item->cornerRadChanged(item_rx,item_ry);
}

void RightSideBar::itemWEdited(int v)
{
    item_w = v;
    QSpinBox* new_item = dynamic_cast<QSpinBox*>(QObject::sender());
    if(new_item->hasFocus()){
        if(rect_item != nullptr)
            rect_item->wChanged(item_w);
        if(ellipse_item != nullptr)
            ellipse_item->wChanged(item_w);
        qDebug() << "W_EDITED";
    }
}

void RightSideBar::itemHEdited(int v)
{
    item_h = v;
    QSpinBox* new_item = dynamic_cast<QSpinBox*>(QObject::sender());
    if(new_item->hasFocus()){
        if(rect_item != nullptr)
            rect_item->hChanged(item_h);
        if(ellipse_item != nullptr)
            ellipse_item->hChanged(item_h);
    }
}

//=========================================================================================================
void RightSideBar::setupPositionPropBox(RightSideBar::ShowMode mode)
{
    if(positionProp != nullptr && positionPropTemp != nullptr){
        this->removeAction(positionPropTemp);
        positionProp = nullptr;
        positionPropTemp = nullptr;
    }

    positionProp = new QGroupBox("Position", this);

    QGridLayout *layout = new QGridLayout;

    if(mode == Rect){
        QHBoxLayout *w_layout = new QHBoxLayout();

        QLabel *w_label = new QLabel("W");
        w_layout->addWidget(w_label);

        QSpinBox *w_edit = new QSpinBox();
        w_edit->setRange(0, 10000);
        w_edit->setValue(item_w);
        connect(w_edit, SIGNAL(valueChanged(int)), this, SLOT(itemWEdited(int)));
        connect(rect_item,SIGNAL(wChangedSignal(int)), w_edit, SLOT(setValue(int)));
        w_layout->addWidget(w_edit);

        layout->addItem(w_layout,0,0);


        QHBoxLayout *h_layout = new QHBoxLayout();

        QLabel *h_label = new QLabel("H");
        h_layout->addWidget(h_label);

        QSpinBox *h_edit = new QSpinBox();
        h_edit->setRange(0, 10000);
        h_edit->setValue(item_h);
        connect(h_edit, SIGNAL(valueChanged(int)), this, SLOT(itemHEdited(int)));
        connect(rect_item,SIGNAL(hChangedSignal(int)), h_edit, SLOT(setValue(int)));
        h_layout->addWidget(h_edit);

        layout->addItem(h_layout,1,0);


        QHBoxLayout *x_layout = new QHBoxLayout();

        QLabel *x_label = new QLabel("X");
        x_layout->addWidget(x_label);

        QSpinBox *x_edit = new QSpinBox();
        x_edit->setRange(0, 10000);
        x_edit->setValue(item_x);
        connect(x_edit, SIGNAL(valueChanged(int)), this, SLOT(itemXEdited(int)));
        connect(rect_item,SIGNAL(xChangedSignal(int)), x_edit, SLOT(setValue(int)));
        x_layout->addWidget(x_edit);

        layout->addItem(x_layout,0,1);


        QHBoxLayout *y_layout = new QHBoxLayout();

        QLabel *y_label = new QLabel("Y");
        y_layout->addWidget(y_label);

        QSpinBox *y_edit = new QSpinBox();
        y_edit->setRange(0, 10000);
        y_edit->setValue(item_y);
        connect(y_edit, SIGNAL(valueChanged(int)), this, SLOT(itemYEdited(int)));
        connect(rect_item,SIGNAL(yChangedSignal(int)), y_edit, SLOT(setValue(int)));
        y_layout->addWidget(y_edit);

        layout->addItem(y_layout,1,1);
    }

    if(mode == Ellipse){
        QHBoxLayout *w_layout = new QHBoxLayout();

        QLabel *w_label = new QLabel("W");
        w_layout->addWidget(w_label);

        QSpinBox *w_edit = new QSpinBox();
        w_edit->setRange(0, 10000);
        w_edit->setValue(item_w);
        connect(w_edit, SIGNAL(valueChanged(int)), this, SLOT(itemWEdited(int)));
        connect(ellipse_item,SIGNAL(wChangedSignal(int)), w_edit, SLOT(setValue(int)));
        w_layout->addWidget(w_edit);

        layout->addItem(w_layout,0,0);


        QHBoxLayout *h_layout = new QHBoxLayout();

        QLabel *h_label = new QLabel("H");
        h_layout->addWidget(h_label);

        QSpinBox *h_edit = new QSpinBox();
        h_edit->setRange(0, 10000);
        h_edit->setValue(item_h);
        connect(h_edit, SIGNAL(valueChanged(int)), this, SLOT(itemHEdited(int)));
        connect(ellipse_item,SIGNAL(hChangedSignal(int)), h_edit, SLOT(setValue(int)));
        h_layout->addWidget(h_edit);

        layout->addItem(h_layout,1,0);


        QHBoxLayout *x_layout = new QHBoxLayout();

        QLabel *x_label = new QLabel("X");
        x_layout->addWidget(x_label);

        QSpinBox *x_edit = new QSpinBox();
        x_edit->setRange(0, 10000);
        x_edit->setValue(item_x);
        connect(x_edit, SIGNAL(valueChanged(int)), this, SLOT(itemXEdited(int)));
        connect(ellipse_item,SIGNAL(xChangedSignal(int)), x_edit, SLOT(setValue(int)));
        x_layout->addWidget(x_edit);

        layout->addItem(x_layout,0,1);


        QHBoxLayout *y_layout = new QHBoxLayout();

        QLabel *y_label = new QLabel("Y");
        y_layout->addWidget(y_label);

        QSpinBox *y_edit = new QSpinBox();
        y_edit->setRange(0, 10000);
        y_edit->setValue(item_y);
        connect(y_edit, SIGNAL(valueChanged(int)), this, SLOT(itemYEdited(int)));
        connect(ellipse_item,SIGNAL(yChangedSignal(int)), y_edit, SLOT(setValue(int)));
        y_layout->addWidget(y_edit);

        layout->addItem(y_layout,1,1);
    }

    if(mode == Line){
        QHBoxLayout *first_layout = new QHBoxLayout();

        QLabel *x1_label = new QLabel("X1");
        first_layout->addWidget(x1_label);

        QSpinBox *x1_edit = new QSpinBox();
        x1_edit->setRange(0, 10000);
        x1_edit->setValue(item_x1);
        connect(x1_edit, SIGNAL(valueChanged(int)), this, SLOT(itemX1Edited(int)));
        connect(line_item,SIGNAL(x1ChangedSignal(int)), x1_edit, SLOT(setValue(int)));
        first_layout->addWidget(x1_edit);

        QLabel *y1_label = new QLabel("Y1");
        first_layout->addWidget(y1_label);

        QSpinBox *y1_edit = new QSpinBox();
        y1_edit->setRange(0, 10000);
        y1_edit->setValue(item_x1);
        connect(y1_edit, SIGNAL(valueChanged(int)), this, SLOT(itemY1Edited(int)));
        connect(line_item,SIGNAL(y1ChangedSignal(int)), y1_edit, SLOT(setValue(int)));
        first_layout->addWidget(y1_edit);

        layout->addItem(first_layout,0,0);


        QHBoxLayout *second_layout = new QHBoxLayout();

        QLabel *x2_label = new QLabel("X2");
        second_layout->addWidget(x2_label);

        QSpinBox *x2_edit = new QSpinBox();
        x2_edit->setRange(0, 10000);
        x2_edit->setValue(item_x2);
        connect(x2_edit, SIGNAL(valueChanged(int)), this, SLOT(itemX2Edited(int)));
        connect(line_item,SIGNAL(x2ChangedSignal(int)), x2_edit, SLOT(setValue(int)));
        second_layout->addWidget(x2_edit);

        QLabel *y2_label = new QLabel("Y2");
        second_layout->addWidget(y2_label);

        QSpinBox *y2_edit = new QSpinBox();
        y2_edit->setRange(0, 10000);
        y2_edit->setValue(item_x2);
        connect(y2_edit, SIGNAL(valueChanged(int)), this, SLOT(itemY2Edited(int)));
        connect(line_item,SIGNAL(y2ChangedSignal(int)), y2_edit, SLOT(setValue(int)));
        second_layout->addWidget(y2_edit);

        layout->addItem(second_layout,1,0);
    }

    positionProp->setLayout(layout);
}

//=========================================================================================================
void RightSideBar::setupAppearencePropBox(RightSideBar::ShowMode mode)
{
    if(appearenceProp != nullptr && appearencePropTemp != nullptr){
        this->removeAction(appearencePropTemp);
        appearenceProp = nullptr;
        appearencePropTemp = nullptr;
    }

    appearenceProp = new QGroupBox("Appearence", this);

    QGridLayout *layout = new QGridLayout;

    if(mode == Line){
        QHBoxLayout *stroke_layout = new QHBoxLayout();



        QLabel *stroke_label = new QLabel("Stroke");
        stroke_layout->addWidget(stroke_label);

        QPushButton *stroke_color = new QPushButton();
        stroke_color->setStyleSheet("background-color: " + item_stroke_color.name());
        connect(stroke_color, &QAbstractButton::clicked, this, itemStrokeColorEdited);
        stroke_layout->addWidget(stroke_color);

        QLabel *stroke_w_label = new QLabel("w");
        stroke_layout->addWidget(stroke_w_label);

        QSpinBox *stroke_w_edit = new QSpinBox();
        stroke_w_edit->setRange(0, 10000);
        stroke_w_edit->setValue(item_stroke_w);
        connect(stroke_w_edit, SIGNAL(valueChanged(int)), this, SLOT(itemStrokeWEdited(int)));
        stroke_layout->addWidget(stroke_w_edit);

        QLabel *stroke_op_label = new QLabel("opacity");
        stroke_layout->addWidget(stroke_op_label);

        QSpinBox *stroke_op_edit = new QSpinBox();
        stroke_op_edit->setRange(0, 255);
        stroke_op_edit->setValue(item_stroke_color.alpha());
        connect(this, SIGNAL(setStrokeOpacitySignal(int)), stroke_op_edit, SLOT(setValue(int)));
        connect(stroke_op_edit, SIGNAL(valueChanged(int)), this, SLOT(itemStrokeOpacityEdited(int)));
        stroke_layout->addWidget(stroke_op_edit);

        layout->addItem(stroke_layout,0,0);
    }

    if(mode == Ellipse){
        QHBoxLayout *stroke_layout = new QHBoxLayout();


        QLabel *stroke_label = new QLabel("Stroke");
        stroke_layout->addWidget(stroke_label);

        QPushButton *stroke_color = new QPushButton();
        stroke_color->setStyleSheet("background-color: " + item_stroke_color.name());
        connect(stroke_color, &QAbstractButton::clicked, this, itemStrokeColorEdited);
        stroke_layout->addWidget(stroke_color);

        QLabel *stroke_w_label = new QLabel("w");
        stroke_layout->addWidget(stroke_w_label);

        QSpinBox *stroke_w_edit = new QSpinBox();
        stroke_w_edit->setRange(0, 10000);
        stroke_w_edit->setValue(item_stroke_w);
        connect(stroke_w_edit, SIGNAL(valueChanged(int)), this, SLOT(itemStrokeWEdited(int)));
        stroke_layout->addWidget(stroke_w_edit);

        QLabel *stroke_op_label = new QLabel("opacity");
        stroke_layout->addWidget(stroke_op_label);

        QSpinBox *stroke_op_edit = new QSpinBox();
        stroke_op_edit->setRange(0, 255);
        stroke_op_edit->setValue(item_stroke_color.alpha());
        connect(this, SIGNAL(setStrokeOpacitySignal(int)), stroke_op_edit, SLOT(setValue(int)));
        connect(stroke_op_edit, SIGNAL(valueChanged(int)), this, SLOT(itemStrokeOpacityEdited(int)));
        stroke_layout->addWidget(stroke_op_edit);

        layout->addItem(stroke_layout,0,0);


        QHBoxLayout *fill_layout = new QHBoxLayout();

        QLabel *fill_label = new QLabel("Fill");
        fill_layout->addWidget(fill_label);

        QPushButton *fill_color = new QPushButton();
        fill_color->setStyleSheet("background-color: " + item_fill_color.name());
        connect(fill_color, &QAbstractButton::clicked, this, itemFillColorEdited);
        fill_layout->addWidget(fill_color);

        QLabel *fill_op_label = new QLabel("opacity");
        fill_layout->addWidget(fill_op_label);

        QSpinBox *fill_op_edit = new QSpinBox();
        fill_op_edit->setRange(0, 255);
        fill_op_edit->setValue(item_fill_color.alpha());
        connect(this, SIGNAL(setFillOpacitySignal(int)), fill_op_edit, SLOT(setValue(int)));
        connect(fill_op_edit, SIGNAL(valueChanged(int)), this, SLOT(itemFillOpacityEdited(int)));
        fill_layout->addWidget(fill_op_edit);

        layout->addItem(fill_layout,1,0);
    }

    if(mode == Rect){
        QHBoxLayout *stroke_layout = new QHBoxLayout();



        QLabel *stroke_label = new QLabel("Stroke");
        stroke_layout->addWidget(stroke_label);

        QPushButton *stroke_color = new QPushButton();
        stroke_color->setStyleSheet("background-color: " + item_stroke_color.name());
        connect(stroke_color, &QAbstractButton::clicked, this, itemStrokeColorEdited);
        stroke_layout->addWidget(stroke_color);

        QLabel *stroke_w_label = new QLabel("w");
        stroke_layout->addWidget(stroke_w_label);

        QSpinBox *stroke_w_edit = new QSpinBox();
        stroke_w_edit->setRange(0, 10000);
        stroke_w_edit->setValue(item_stroke_w);
        connect(stroke_w_edit, SIGNAL(valueChanged(int)), this, SLOT(itemStrokeWEdited(int)));
        stroke_layout->addWidget(stroke_w_edit);

        QLabel *stroke_op_label = new QLabel("opacity");
        stroke_layout->addWidget(stroke_op_label);


        QSpinBox *stroke_op_edit = new QSpinBox();
        stroke_op_edit->setRange(0, 255);
        stroke_op_edit->setValue(item_stroke_color.alpha());
        connect(this, SIGNAL(setStrokeOpacitySignal(int)), stroke_op_edit, SLOT(setValue(int)));
        connect(stroke_op_edit, SIGNAL(valueChanged(int)), this, SLOT(itemStrokeOpacityEdited(int)));
        stroke_layout->addWidget(stroke_op_edit);

        layout->addItem(stroke_layout,0,0);


        QHBoxLayout *fill_layout = new QHBoxLayout();

        QLabel *fill_label = new QLabel("Fill");
        fill_layout->addWidget(fill_label);

        QPushButton *fill_color = new QPushButton();
        fill_color->setStyleSheet("background-color: " + item_fill_color.name());
        connect(fill_color, &QAbstractButton::clicked, this, itemFillColorEdited);
        fill_layout->addWidget(fill_color);

        QLabel *fill_op_label = new QLabel("opacity");
        fill_layout->addWidget(fill_op_label);

        QSpinBox *fill_op_edit = new QSpinBox();
        fill_op_edit->setRange(0, 255);
        fill_op_edit->setValue(item_fill_color.alpha());
        connect(this, SIGNAL(setFillOpacitySignal(int)), fill_op_edit, SLOT(setValue(int)));
        connect(fill_op_edit, SIGNAL(valueChanged(int)), this, SLOT(itemFillOpacityEdited(int)));
        fill_layout->addWidget(fill_op_edit);

        layout->addItem(fill_layout,1,0);


        QHBoxLayout *r_layout = new QHBoxLayout();

        QLabel *rx_label = new QLabel("rx");
        r_layout->addWidget(rx_label);

        QSpinBox *rx_edit = new QSpinBox();
        rx_edit->setRange(0, 10000);
        rx_edit->setValue(item_rx);
        connect(rx_edit, SIGNAL(valueChanged(int)), this, SLOT(itemRXEdited(int)));
        r_layout->addWidget(rx_edit);

        QLabel *ry_label = new QLabel("ry");
        r_layout->addWidget(ry_label);

        QSpinBox *ry_edit = new QSpinBox();
        ry_edit->setRange(0, 10000);
        ry_edit->setValue(item_ry);
        connect(ry_edit, SIGNAL(valueChanged(int)), this, SLOT(itemRYEdited(int)));
        r_layout->addWidget(ry_edit);

        layout->addItem(r_layout,2,0);
    }

    appearenceProp->setLayout(layout);
}
