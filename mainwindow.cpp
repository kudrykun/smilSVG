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
}

MainWindow::~MainWindow()
{
    delete ui;
}
