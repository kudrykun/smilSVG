#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QFileDialog>
#include "parsertohtml.h"

//=========================================================================================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    resize(1500,1000);
    scene = new EditorScene(this);
    view = new EditorView(scene, this);
    setCentralWidget(view);


    auto groupping = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_G),this,SIGNAL(groupAction()));
    connect(this, SIGNAL(groupAction()), scene, SLOT(groupAction()));
    shortcuts.push_back(groupping);

    auto ungroupping = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_U),this,SIGNAL(ungroupAction()));
    connect(this, SIGNAL(ungroupAction()), scene, SLOT(ungroupAction()));
    shortcuts.push_back(ungroupping);

    auto unselectTool = new QShortcut(QKeySequence(Qt::Key_Escape),this,SIGNAL(unselectToolSignal()));
    connect(this, SIGNAL(unselectToolSignal()), this, SLOT(unselectToolSlot()));
    shortcuts.push_back(unselectTool);

    this->createItemsActionGroup();
    this->createEditionActionGroup();
    this->createItemsToolbar();
    this->createRightSideBar();
    this->createMenus();
}

//=========================================================================================================
MainWindow::~MainWindow()
{
    delete ui;
}

//=========================================================================================================
void MainWindow::unselectToolSlot()
{
    if(itemsActionGroup != 0)
        for(auto &tool : itemsActionGroup->actions())
            tool->setChecked(false);

    QApplication::restoreOverrideCursor();
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    this->view->setItemToolState(EditorView::None);
}

void MainWindow::addLineActionToggled(bool)
{
    this->view->setItemToolState(EditorView::LineTool);
    QApplication::restoreOverrideCursor();
    QApplication::setOverrideCursor(Qt::CrossCursor);
}

void MainWindow::addRectActionToggled(bool)
{
    this->view->setItemToolState(EditorView::RectTool);
    QApplication::restoreOverrideCursor();
    QApplication::setOverrideCursor(Qt::CrossCursor);
}

void MainWindow::addEllipseActionToggled(bool)
{
    this->view->setItemToolState(EditorView::EllipseTool);
    QApplication::restoreOverrideCursor();
    QApplication::setOverrideCursor(Qt::CrossCursor);
}


void MainWindow::saveToFileSlot()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save to HTML file"), "",
            tr("HTML file (*.html);;All Files (*)"));

    if (fileName.isEmpty())
            return;
    else {
        ParserToHtml *parser = new ParserToHtml(scene,fileName);
        qDebug() << parser->getText();
        QFile file(fileName);
        if (file.open(QIODevice::ReadWrite)) {
                QTextStream stream(&file);
                stream << parser->getText() << endl;
        }
    }
}

//=========================================================================================================
void MainWindow::createItemsActionGroup()
{
    QAction *add_line = new QAction(QIcon(":/icons/Resources/icons/line_icon.png"),"Line", this);
    add_line->setShortcut(QKeySequence(Qt::Key_L));
    add_line->setCheckable(true);
    connect(add_line, SIGNAL(toggled(bool)), this, SLOT(addLineActionToggled(bool)));

    QAction *add_rect = new QAction(QIcon(":/icons/Resources/icons/rectangle_icon.png"),"Rectangle", this);
    add_rect->setShortcut(QKeySequence(Qt::Key_R));
    add_rect->setCheckable(true);
    connect(add_rect, SIGNAL(toggled(bool)), this, SLOT(addRectActionToggled(bool)));

    QAction *add_ellipse = new QAction(QIcon(":/icons/Resources/icons/ellipse_icon.png"),"Ellipse", this);
    add_ellipse->setShortcut(QKeySequence(Qt::Key_E));
    add_ellipse->setCheckable(true);
    connect(add_ellipse, SIGNAL(toggled(bool)), this, SLOT(addEllipseActionToggled(bool)));

    itemsActionGroup = new QActionGroup(this);
    itemsActionGroup->setExclusive(true);
    itemsActionGroup->addAction(add_line);
    itemsActionGroup->addAction(add_rect);
    itemsActionGroup->addAction(add_ellipse);
}

void MainWindow::createEditionActionGroup()
{
    QAction *delete_item = new QAction("Delete item", this);
    delete_item->setShortcut(QKeySequence(Qt::Key_Delete));
    connect(delete_item, SIGNAL(triggered()), view, SLOT(deleteItem()));

    QAction *duplicate_item = new QAction("Duplicate item", this);
    duplicate_item->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
    connect(duplicate_item, SIGNAL(triggered()), view, SLOT(duplicateItem()));

    editionActionGroup = new QActionGroup(this);
    editionActionGroup->addAction(delete_item);
    editionActionGroup->addAction(duplicate_item);
}

//=========================================================================================================
void MainWindow::createItemsToolbar()
{
   QToolBar *itemBar = new QToolBar("Items Toolbar", this);
   auto label = new QLabel("Items", itemBar);
   label->setAlignment(Qt::AlignCenter);
   itemBar->addWidget(label);
   itemBar->addSeparator();
   for(auto &tool : itemsActionGroup->actions())
       itemBar->addAction(tool);

   for(auto &tool : editionActionGroup->actions())
       itemBar->addAction(tool);

   this->addToolBar(Qt::LeftToolBarArea, itemBar);
}

//=========================================================================================================
void MainWindow::createMenus()
{
    parseToAction = new QAction(tr("Parse to file"), this);
    parseToAction->setStatusTip(tr("Create a new HTML document"));
    connect(parseToAction, &QAction::triggered, this, &MainWindow::saveToFileSlot);

    fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(parseToAction);
}

//=========================================================================================================
void MainWindow::createRightSideBar(QGraphicsItem *item)
{
    if(rightBar != nullptr){
        this->removeToolBar(rightBar);
        rightBar = nullptr;
    }

    rightBar = new RightSideBar(item, this);

    this->addToolBar(Qt::RightToolBarArea, rightBar);
}
