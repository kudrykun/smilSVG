#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QAction>
#include <QActionGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include "sidebar.h"
#include "editorscene.h"
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
    // панель с выбором инструмента
    void createToolBarActions();
    // панелька свойств
    void createSideBar();

private:
    Ui::MainWindow *ui;

    QGraphicsView *view;
    EditorScene *scene;
    QGraphicsRectItem *rectangle = 0;

    QActionGroup *toolBar = 0;
    QHBoxLayout *container = 0;
    SideBar *sidebar = 0;
    QAction *pointerTool = 0;
    QAction *rectangleTool = 0;
    QAction *ellipseTool = 0;
    QAction *lineTool = 0;
};

#endif // MAINWINDOW_H
