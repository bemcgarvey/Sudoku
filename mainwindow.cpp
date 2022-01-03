#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPrintDialog>
#include <QPrinter>

//TODO Add save and load
//TODO Add solve

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), m_invalidAttempts(0)
{
    ui->setupUi(this);
    ui->gridView->setGrid(new Grid());
    connect(ui->gridView, &GridView::lockedCell, this, &MainWindow::onLockedEntry);
    connect(ui->gridView, &GridView::invalidEntry, this, &MainWindow::onInvalidEntry);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLockedEntry()
{
    ui->statusbar->showMessage("Locked Cell", 2000);
}

void MainWindow::onInvalidEntry()
{
    ui->statusbar->showMessage("Invalid Entry", 2000);
    ++m_invalidAttempts;
    //TODO show invalid attempts?
}


void MainWindow::on_actionExit_triggered()
{
    close();
}


void MainWindow::on_actionLock_Editing_triggered()
{
    ui->gridView->lockGrid(true);
    m_invalidAttempts = 0;
    update();
}


void MainWindow::on_actionUnlock_Editing_triggered()
{
    ui->gridView->lockGrid(false);
    update();
}


void MainWindow::on_actionNew_triggered()
{
    ui->gridView->setGrid(new Grid());
    update();
}


void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    std::unique_ptr<QPrintDialog> dlg(new QPrintDialog(&printer, this));
    if (dlg->exec() == QDialog::Accepted) {
        ui->gridView->print(&printer);
    }
}

