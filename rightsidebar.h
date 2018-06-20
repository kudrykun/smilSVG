#ifndef RIGHTSIDEBAR_H
#define RIGHTSIDEBAR_H

#include <QToolBar>
#include <QGroupBox>

class RightSideBar : public QToolBar
{
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

    RightSideBar(ShowMode initialMode, QWidget *parent = nullptr);
    void setShowMode(ShowMode m) {showMode = m;}
    ShowMode getShowMode() {return showMode;}
public slots:
    void setColor();

signals:
    void colorBtnChanged(QColor color);

private:
    void setupPositionPropBox(ShowMode mode);
    void setupAppearencePropBox(ShowMode mode);

private:
    ShowMode showMode;
    QGroupBox *positionProp = nullptr;
    QAction *positionPropTemp = nullptr;
    QGroupBox *appearenceProp = nullptr;
    QAction *appearencePropTemp = nullptr;
};

#endif // RIGHTSIDEBAR_H
