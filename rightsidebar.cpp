#include "rightsidebar.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>
#include <QPushButton>
#include <QColorDialog>
#include <QDebug>

RightSideBar::RightSideBar(ShowMode initialMode, QWidget *parent) : QToolBar(parent)
{
    showMode = initialMode;

    setupPositionPropBox(showMode);
    positionPropTemp = this->addWidget(positionProp);

    setupAppearencePropBox(showMode);
    appearencePropTemp = this->addWidget(appearenceProp);
}

void RightSideBar::setColor()
{
    //const QColorDialog::ColorDialogOptions options = QFlag(colorDialogOptionsWidget->value());
    qDebug() << "before dialog";
    const QColor color = QColorDialog::getColor(Qt::green, this, "Select Color");
    qDebug() << "after dialog";
    if (color.isValid()) {
        colorBtnChanged(color);
    }
}

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

void RightSideBar::setupAppearencePropBox(RightSideBar::ShowMode mode)
{
    if(appearenceProp != nullptr && appearencePropTemp != nullptr){
        this->removeAction(appearencePropTemp);
        positionProp = nullptr;
        appearencePropTemp = nullptr;
    }

    appearenceProp = new QGroupBox("Position", this);

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
        QHBoxLayout *stroke_layout = new QHBoxLayout();
        QLabel *stroke_label = new QLabel("Stroke");
        QPushButton *stroke_color = new QPushButton();
        stroke_color->setStyleSheet("background-color: red");
        connect(stroke_color, &QAbstractButton::clicked, this, SLOT(setColor()));
        connect(this, SIGNAL(colorBtnChanged(QColor)), stroke_color, [this, &stroke_color](QColor r){stroke_color->setStyleSheet("background-color: " + r.name());});
        QLineEdit *stroke_edit = new QLineEdit("8");
        stroke_edit->setValidator(new QIntValidator(0,10000));
        stroke_layout->addWidget(stroke_label);
        stroke_layout->addWidget(stroke_color);
        stroke_layout->addWidget(stroke_edit);
        layout->addItem(stroke_layout,0,0);
    }

    appearenceProp->setLayout(layout);
}
