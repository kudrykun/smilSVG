#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "editorscene.h"
#include "editorview.h"

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
private:
    Ui::MainWindow *ui;

    EditorView *view;
    EditorScene *scene;
};

#endif // MAINWINDOW_H
