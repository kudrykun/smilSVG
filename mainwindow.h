#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "editorscene.h"
#include "editorview.h"
#include <QShortcut>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    EditorScene* getMainScene() {return scene;}

signals:
    void groupAction();

private:
    Ui::MainWindow *ui;

    EditorView *view;
    EditorScene *scene;

    QVector<QShortcut*> shortcuts;
};

#endif // MAINWINDOW_H
