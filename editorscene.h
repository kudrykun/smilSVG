#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>

class EditorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode {Pointer, RectItem, EllipseItem, LineItem};
    enum KeyMode {None, Shift};
    EditorScene(QObject *parent = 0);

public slots:
    void setMode(int mode) {currentMode = static_cast<Mode>(mode);} //установка текущего режима

    void setWidth(QString text);
    void setHeight(int num);

    void setX(int num);
    void setY(int num);

signals:
    void widthChanged(int num);
    void heightChanged(int num);
    void xChanged(int num);
    void yChanged(int num);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

protected:
    void drawRect(QRect rect);
    void drawEllipse(QRect rect);
    void drawLine(QRect rect);

    void selectItem(QGraphicsItem *item);

private:
    Mode currentMode;
    KeyMode currentKeyMode;
    QColor currentItemColor;
    QGraphicsItem *temp_item = 0;
    QGraphicsItem *selectedItem = 0;
    int sx = 0;
    int sy = 0;
    int cx = 0;
    int cy = 0;
};

#endif // EDITORSCENE_H
