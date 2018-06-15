#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSignalMapper>
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    resize(1500,1000);
    scene = new EditorScene(this);
    view = new EditorView(scene, this);
    setCentralWidget(view);

   // shortcuts.push_back(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_G),this,SLOT(scene->groupAction())));
    auto groupping = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_G),this,SIGNAL(groupAction()));
    connect(this, SIGNAL(groupAction()), scene, SLOT(groupAction()));
    shortcuts.push_back(groupping);
}

MainWindow::~MainWindow()
{
    delete ui;
}
