#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSize>

class SideBar : public QWidget
{
    Q_OBJECT
public:
    explicit SideBar(QWidget* parent = 0);
    //QSize sizeHint() override {return QSize(400,400);}

private slots:
    void setWidth(int num);
    void setHeight(int num);

    void setX(int num);
    void setY(int num);

signals:
     void widthChanged(QString text);
     void heightChanged(int num);
     void xChanged(int num);
     void yChanged(int num);

private:
    QVBoxLayout *sidebar = 0;

    QLineEdit *width_edit = 0;
    QLineEdit *height_edit = 0;
    QLineEdit *x_edit = 0;
    QLineEdit *y_edit = 0;

};

#endif // SIDEBAR_H
