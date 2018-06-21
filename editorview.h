#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QObject>
#include <QWheelEvent>
#include <QGraphicsView>
#include <QDebug>
#include "editorscene.h"

class EditorView : public QGraphicsView
{
    Q_OBJECT
public:    

    enum ItemToolState{
        None,
        LineTool,
        RectTool,
        EllipseTool
    };

    EditorView(EditorScene *scene, QObject *parent = nullptr);

    void setItemToolState(ItemToolState state) {item_tool_state = state;}
    ItemToolState getItemToolState() {return item_tool_state;}

signals:
    void scaleChanged(qreal);

public slots:
    void deleteItem();
    void duplicateItem();

protected:
    //переопределяем для зума сцены
    void wheelEvent(QWheelEvent *ev);

    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

private:
    qreal current_scale = 1;
    bool space_pressed = false;
    bool lmb_pressed = false;
    QPointF previous_mouse_pos;

    ItemToolState item_tool_state = None;
    QGraphicsItem *drawing_item;
    QRectF drawing_rect;
};

#endif // EDITORVIEW_H
