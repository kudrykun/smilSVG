#include "sidebar.h"
#include <QLabel>
#include <QDebug>
SideBar::SideBar(QWidget *parent) : QWidget(parent)
{
      setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
      sidebar = new QVBoxLayout();
      sidebar->setMargin(11);
      sidebar->setSpacing(11);

      width_edit = new QLineEdit();
      connect(width_edit, SIGNAL(textEdited(QString)), this, SIGNAL(widthChanged(QString)));
      QLabel *width_lbl = new QLabel("Width",width_edit);
      QVBoxLayout *width_l = new QVBoxLayout();
      width_l->addWidget(width_lbl);
      width_l->addWidget(width_edit);

      height_edit = new QLineEdit();
      QLabel *height_lbl = new QLabel("Height",height_edit);
      QVBoxLayout *height_l = new QVBoxLayout();
      height_l->addWidget(height_lbl);
      height_l->addWidget(height_edit);

      x_edit = new QLineEdit();
      QLabel *x_lbl = new QLabel("X",x_edit);
      QVBoxLayout *x_l = new QVBoxLayout();
      x_l->addWidget(x_lbl);
      x_l->addWidget(x_edit);

      y_edit = new QLineEdit();
      QLabel *y_lbl = new QLabel("Y",y_edit);
      QVBoxLayout *y_l = new QVBoxLayout();
      y_l->addWidget(y_lbl);
      y_l->addWidget(y_edit);

      QGridLayout *position_layout = new QGridLayout();
      position_layout->addItem(width_l, 0,0);
      position_layout->addItem(x_l, 0,1);
      position_layout->addItem(height_l, 1,0);
      position_layout->addItem(y_l, 1,1);
      sidebar->addLayout(position_layout);
      sidebar->addStretch(1);

      setLayout(sidebar);
}


void SideBar::setWidth(int num)
{
    width_edit->setText(QString::number(num));
}

void SideBar::setHeight(int num)
{
    height_edit->setText(QString::number(num));
}

void SideBar::setX(int num)
{
    x_edit->setText(QString::number(num));
}

void SideBar::setY(int num)
{
    y_edit->setText(QString::number(num));
}
