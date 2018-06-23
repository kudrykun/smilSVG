#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "editorscene.h"
#include "editorview.h"
#include <QShortcut>
#include <QActionGroup>
#include "rightsidebar.h"

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
    void createRightSideBar(QGraphicsItem *item = nullptr);

signals:
    void groupAction();
    void ungroupAction();
    void unselectToolSignal();
    void deleteItem();
    void duplicateItem();

public slots:
    void unselectToolSlot();
    void addLineActionToggled(bool);
    void addRectActionToggled(bool);
    void addEllipseActionToggled(bool);

private:
    void createItemsActionGroup();
    void createEditionActionGroup();
    void createItemsToolbar();
    void createMenus();

private:
    Ui::MainWindow *ui;

    EditorView *view;
    EditorScene *scene;

    QVector<QShortcut*> shortcuts;

    QActionGroup *itemsActionGroup;
    QActionGroup *editionActionGroup;

    RightSideBar *rightBar = nullptr;
    QMenu *fileMenu = nullptr;
};

#endif // MAINWINDOW_H
