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


    auto groupping = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_G),this,SIGNAL(groupAction()));
    connect(this, SIGNAL(groupAction()), scene, SLOT(groupAction()));
    shortcuts.push_back(groupping);

    auto ungroupping = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_U),this,SIGNAL(ungroupAction()));
    connect(this, SIGNAL(ungroupAction()), scene, SLOT(ungroupAction()));
    shortcuts.push_back(ungroupping);
}

MainWindow::~MainWindow()
{
    delete ui;
}
