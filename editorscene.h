#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include "svgdocument.h"

class EditorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode {Pointer, Rect, Ellipse, Line}; // выборы инструмента
    enum KeyMode {None, Shift}; //модификатор инструмента
    EditorScene(QObject *parent = 0);
    void setCurrentScale(qreal scale);

public slots:
    void setMode(int mode) {currentMode = static_cast<Mode>(mode);} //установка инструмента
    void updateScale(qreal scale);
    void groupAction();
protected:
    //переопределнный метод, используемый для отрисовки сетки
    void drawBackground(QPainter *painter, const QRectF &rect);
signals:
    void updateDocumentScale(qreal scale);

private:
    void drawTestFigures();
    void setupHotkeys();

private:
    Mode currentMode; //текущий инструмент
    KeyMode currentKeyMode; //текущий модификатор

    SvgDocument *document;
    QColor sceneColor = QColor(228,228,228);
    qreal current_scale;

    bool debug_mode = true;
};

#endif // EDITORSCENE_H
