#include "mainwindow.h"
#include "ui_mainwindow.h"

//TODO Add save and load
//TODO Add new
//TODO Add solve

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->gridView->setGrid(new Grid());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionExit_triggered()
{
    close();
}


void MainWindow::on_actionLock_Editing_triggered()
{
    ui->gridView->lockGrid(true);
    update();
}

