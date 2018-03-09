#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QDebug>
#include <QSignalMapper>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //это мы так устанавливаем размеры экрана
    QSize availableSize = qApp->desktop()->availableGeometry().size();
    int width = availableSize.width();
    int height = availableSize.height();
    width *= 0.7; // 90% of the screen size
    height *= 0.7; // 90% of the screen size
    QSize newSize( width, height );
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            newSize,
            qApp->desktop()->availableGeometry()
        )
    );


   view = new QGraphicsView();
   scene = new EditorScene();
   view->setScene(scene);

   container = new QHBoxLayout();
   container->setMargin(11);
   container->setSpacing(11);
   container->addWidget(view);

   QWidget *widget = new QWidget();
   widget->setLayout(container);
   setCentralWidget(widget);

   createToolBarActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::createToolBarActions()
{
    toolBar = new QActionGroup(ui->mainToolBar);

    rectangleTool = toolBar->addAction("Rectangle");
    rectangleTool->setCheckable(true);

    ellipseTool = toolBar->addAction("Ellipse");
    ellipseTool->setCheckable(true);

    lineTool = toolBar->addAction("Line");
    lineTool->setCheckable(true);

    pointerTool = toolBar->addAction("Pointer");
    pointerTool->setCheckable(true);

    QSignalMapper *mapper = new QSignalMapper(toolBar);

    connect(rectangleTool, SIGNAL(triggered(bool)), mapper, SLOT(map()));
    connect(ellipseTool, SIGNAL(triggered(bool)), mapper, SLOT(map()));
    connect(lineTool, SIGNAL(triggered(bool)), mapper, SLOT(map()));
    connect(pointerTool, SIGNAL(triggered(bool)), mapper, SLOT(map()));

    mapper->setMapping(rectangleTool, EditorScene::RectItem);
    mapper->setMapping(ellipseTool, EditorScene::EllipseItem);
    mapper->setMapping(lineTool, EditorScene::LineItem);
    mapper->setMapping(pointerTool, EditorScene::Pointer);

    connect(mapper, SIGNAL(mapped(int)), scene, SLOT(setMode(int)));

    ui->mainToolBar->addActions(toolBar->actions());
}
