#include "rightsidebar.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>
#include <QPushButton>
#include <QColorDialog>
#include <QDebug>

//=========================================================================================================
RightSideBar::RightSideBar(ShowMode initialMode, QWidget *parent) : QToolBar(parent)
{
    showMode = initialMode;

    setupPositionPropBox(showMode);
    positionPropTemp = this->addWidget(positionProp);

    setupAppearencePropBox(showMode);
    appearencePropTemp = this->addWidget(appearenceProp);
}

//=========================================================================================================
void RightSideBar::setStrokeColor()
{
    QColorDialog::ColorDialogOptions options = QFlag(QColorDialog::ShowAlphaChannel);
    const QColor color = QColorDialog::getColor(currentStrokeColor, this, "Select stroke color", options);
    if (color.isValid()) {
        currentStrokeColor = color;

        setupAppearencePropBox(showMode);
        appearencePropTemp = this->addWidget(appearenceProp);

        emit setStrokeOpacitySignal(QString::number(color.alpha()));
    }
}

//=========================================================================================================
void RightSideBar::setFillColor()
{
    QColorDialog::ColorDialogOptions options = QFlag(QColorDialog::ShowAlphaChannel);
    const QColor color = QColorDialog::getColor(currentFillColor, this, "Select fill color",options);
    if (color.isValid()) {
        currentFillColor = color;

        setupAppearencePropBox(showMode);
        appearencePropTemp = this->addWidget(appearenceProp);

        emit setFillOpacitySignal(QString::number(color.alpha()));
    }
}

//=========================================================================================================
void RightSideBar::strokeOpacityEdited(QString op)
{
    currentStrokeColor.setAlpha(op.toInt());
}

//=========================================================================================================
void RightSideBar::fillOpacityEdited(QString op)
{
    currentFillColor.setAlpha(op.toInt());
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

    if(mode == Rect || mode == Ellipse){
        QHBoxLayout *w_layout = new QHBoxLayout();
        QLabel *w_label = new QLabel("W");
        QLineEdit *w_edit = new QLineEdit("8");
        w_edit->setValidator(new QIntValidator(0,10000));
        w_layout->addWidget(w_label);
        w_layout->addWidget(w_edit);
        layout->addItem(w_layout,0,0);

        QHBoxLayout *h_layout = new QHBoxLayout();
        QLabel *h_label = new QLabel("H");
        QLineEdit *h_edit = new QLineEdit("8");
        h_edit->setValidator(new QIntValidator(0,10000));
        h_layout->addWidget(h_label);
        h_layout->addWidget(h_edit);
        layout->addItem(h_layout,1,0);

        QHBoxLayout *x_layout = new QHBoxLayout();
        QLabel *x_label = new QLabel("X");
        QLineEdit *x_edit = new QLineEdit("8");
        x_edit->setValidator(new QIntValidator(0,10000));
        x_layout->addWidget(x_label);
        x_layout->addWidget(x_edit);
        layout->addItem(x_layout,0,1);

        QHBoxLayout *y_layout = new QHBoxLayout();
        QLabel *y_label = new QLabel("Y");
        QLineEdit *y_edit = new QLineEdit("8");
        y_edit->setValidator(new QIntValidator(0,10000));
        y_layout->addWidget(y_label);
        y_layout->addWidget(y_edit);
        layout->addItem(y_layout,1,1);
    }

    if(mode == Line){
        QHBoxLayout *x1_layout = new QHBoxLayout();
        QLabel *x1_label = new QLabel("x1");
        QLineEdit *x1_edit = new QLineEdit("8");
        x1_edit->setValidator(new QIntValidator(0,10000));
        x1_layout->addWidget(x1_label);
        x1_layout->addWidget(x1_edit);
        layout->addItem(x1_layout,0,0);

        QHBoxLayout *y1_layout = new QHBoxLayout();
        QLabel *y1_label = new QLabel("y1");
        QLineEdit *y1_edit = new QLineEdit("8");
        y1_edit->setValidator(new QIntValidator(0,10000));
        y1_layout->addWidget(y1_label);
        y1_layout->addWidget(y1_edit);
        layout->addItem(y1_layout,1,0);

        QHBoxLayout *x2_layout = new QHBoxLayout();
        QLabel *x2_label = new QLabel("x2");
        QLineEdit *x2_edit = new QLineEdit("8");
        x2_edit->setValidator(new QIntValidator(0,10000));
        x2_layout->addWidget(x2_label);
        x2_layout->addWidget(x2_edit);
        layout->addItem(x2_layout,0,1);

        QHBoxLayout *y2_layout = new QHBoxLayout();
        QLabel *y2_label = new QLabel("y2");
        QLineEdit *y2_edit = new QLineEdit("8");
        y2_edit->setValidator(new QIntValidator(0,10000));
        y2_layout->addWidget(y2_label);
        y2_layout->addWidget(y2_edit);
        layout->addItem(y2_layout,1,1);
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
        stroke_color->setStyleSheet("background-color: " + currentStrokeColor.name());
        connect(stroke_color, &QAbstractButton::clicked, this, setStrokeColor);
        stroke_layout->addWidget(stroke_color);

        QLabel *stroke_w_label = new QLabel("w");
        QLineEdit *stroke_w_edit = new QLineEdit("8");
        stroke_w_edit->setValidator(new QIntValidator(0,10000));
        stroke_layout->addWidget(stroke_w_label);
        stroke_layout->addWidget(stroke_w_edit);

        QLabel *stroke_op_label = new QLabel("opacity");
        QLineEdit *stroke_op_edit = new QLineEdit(QString::number(currentStrokeColor.alpha()));
        stroke_op_edit->setValidator(new QIntValidator(0,255));
        connect(this, SIGNAL(setStrokeOpacitySignal(QString)), stroke_op_edit, SLOT(setText(QString)));
        connect(stroke_op_edit, SIGNAL(textEdited(QString)), this, SLOT(strokeOpacityEdited(QString)));
        stroke_layout->addWidget(stroke_op_label);
        stroke_layout->addWidget(stroke_op_edit);

        layout->addItem(stroke_layout,0,0);
    }

    if(mode == Ellipse){
        QHBoxLayout *stroke_layout = new QHBoxLayout();

        QLabel *stroke_label = new QLabel("Stroke");
        stroke_layout->addWidget(stroke_label);

        QPushButton *stroke_color = new QPushButton();
        stroke_color->setStyleSheet("background-color: " + currentStrokeColor.name());
        connect(stroke_color, &QAbstractButton::clicked, this, setStrokeColor);
        stroke_layout->addWidget(stroke_color);

        QLabel *stroke_w_label = new QLabel("w");
        QLineEdit *stroke_w_edit = new QLineEdit("8");
        stroke_w_edit->setValidator(new QIntValidator(0,10000));
        stroke_layout->addWidget(stroke_w_label);
        stroke_layout->addWidget(stroke_w_edit);

        QLabel *stroke_op_label = new QLabel("opacity");
        QLineEdit *stroke_op_edit = new QLineEdit(QString::number(currentStrokeColor.alpha()));
        stroke_op_edit->setValidator(new QIntValidator(0,255));
        connect(this, SIGNAL(setStrokeOpacitySignal(QString)), stroke_op_edit, SLOT(setText(QString)));
        connect(stroke_op_edit, SIGNAL(textEdited(QString)), this, SLOT(strokeOpacityEdited(QString)));
        stroke_layout->addWidget(stroke_op_label);
        stroke_layout->addWidget(stroke_op_edit);

        layout->addItem(stroke_layout,0,0);


        QHBoxLayout *fill_layout = new QHBoxLayout();

        QLabel *fill_label = new QLabel("Fill");
        fill_layout->addWidget(fill_label);

        QPushButton *fill_color = new QPushButton();
        fill_color->setStyleSheet("background-color: " + currentFillColor.name());
        connect(fill_color, &QAbstractButton::clicked, this, setFillColor);
        fill_layout->addWidget(fill_color);

        QLabel *fill_op_label = new QLabel("opacity");
        QLineEdit *fill_op_edit = new QLineEdit(QString::number(currentFillColor.alpha()));
        fill_op_edit->setValidator(new QIntValidator(0,255));
        connect(this, SIGNAL(setFillOpacitySignal(QString)), fill_op_edit, SLOT(setText(QString)));
        connect(fill_op_edit, SIGNAL(textEdited(QString)), this, SLOT(fillOpacityEdited(QString)));
        fill_layout->addWidget(fill_op_label);
        fill_layout->addWidget(fill_op_edit);

        layout->addItem(fill_layout,1,0);
    }

    if(mode == Rect){
        QHBoxLayout *stroke_layout = new QHBoxLayout();

        QLabel *stroke_label = new QLabel("Stroke");
        stroke_layout->addWidget(stroke_label);

        QPushButton *stroke_color = new QPushButton();
        stroke_color->setStyleSheet("background-color: " + currentStrokeColor.name());
        connect(stroke_color, &QAbstractButton::clicked, this, setStrokeColor);
        stroke_layout->addWidget(stroke_color);

        QLabel *stroke_w_label = new QLabel("w");
        QLineEdit *stroke_w_edit = new QLineEdit("8");
        stroke_w_edit->setValidator(new QIntValidator(0,10000));
        stroke_layout->addWidget(stroke_w_label);
        stroke_layout->addWidget(stroke_w_edit);

        QLabel *stroke_op_label = new QLabel("opacity");
        QLineEdit *stroke_op_edit = new QLineEdit(QString::number(currentStrokeColor.alpha()));
        stroke_op_edit->setValidator(new QIntValidator(0,255));
        connect(this, SIGNAL(setStrokeOpacitySignal(QString)), stroke_op_edit, SLOT(setText(QString)));
        connect(stroke_op_edit, SIGNAL(textEdited(QString)), this, SLOT(strokeOpacityEdited(QString)));
        stroke_layout->addWidget(stroke_op_label);
        stroke_layout->addWidget(stroke_op_edit);

        layout->addItem(stroke_layout,0,0);


        QHBoxLayout *fill_layout = new QHBoxLayout();

        QLabel *fill_label = new QLabel("Fill");
        fill_layout->addWidget(fill_label);

        QPushButton *fill_color = new QPushButton();
        fill_color->setStyleSheet("background-color: " + currentFillColor.name());
        connect(fill_color, &QAbstractButton::clicked, this, setFillColor);
        fill_layout->addWidget(fill_color);

        QLabel *fill_op_label = new QLabel("opacity");
        QLineEdit *fill_op_edit = new QLineEdit(QString::number(currentFillColor.alpha()));
        fill_op_edit->setValidator(new QIntValidator(0,255));
        connect(this, SIGNAL(setFillOpacitySignal(QString)), fill_op_edit, SLOT(setText(QString)));
        connect(fill_op_edit, SIGNAL(textEdited(QString)), this, SLOT(fillOpacityEdited(QString)));
        fill_layout->addWidget(fill_op_label);
        fill_layout->addWidget(fill_op_edit);

        layout->addItem(fill_layout,1,0);


        QHBoxLayout *r_layout = new QHBoxLayout();

        QLabel *rx_label = new QLabel("rx");
        QLineEdit *rx_edit = new QLineEdit("4");
        rx_edit->setValidator(new QIntValidator(0,1000));
        r_layout->addWidget(rx_label);
        r_layout->addWidget(rx_edit);

        QLabel *ry_label = new QLabel("ry");
        QLineEdit *ry_edit = new QLineEdit("5");
        ry_edit->setValidator(new QIntValidator(0,1000));
        r_layout->addWidget(ry_label);
        r_layout->addWidget(ry_edit);

        layout->addItem(r_layout,2,0);
    }

    appearenceProp->setLayout(layout);
}
