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
    EditorView(EditorScene *scene, QObject *parent = nullptr);

signals:
    void scaleChanged(qreal);

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
};

#endif // EDITORVIEW_H
