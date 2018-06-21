#ifndef RIGHTSIDEBAR_H
#define RIGHTSIDEBAR_H

#include <QToolBar>
#include <QGroupBox>

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

    RightSideBar(ShowMode initialMode, QWidget *parent = nullptr);
    void setShowMode(ShowMode m) {showMode = m;}
    ShowMode getShowMode() {return showMode;}

signals:
    void setStrokeOpacitySignal(QString op);
    void setFillOpacitySignal(QString op);

public slots:
    void setStrokeColor();
    void setFillColor();

    void strokeOpacityEdited(QString op);
    void fillOpacityEdited(QString op);

private:
    void setupPositionPropBox(ShowMode mode);
    void setupAppearencePropBox(ShowMode mode);

private:
    ShowMode showMode;
    QGroupBox *positionProp = nullptr;
    QAction *positionPropTemp = nullptr;
    QGroupBox *appearenceProp = nullptr;
    QAction *appearencePropTemp = nullptr;

    QColor currentStrokeColor = QColor(255,0,0);
    QColor currentFillColor = QColor(255,0,0);
};

#endif // RIGHTSIDEBAR_H
