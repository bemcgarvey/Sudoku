#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPrintDialog>
#include <QPrinter>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

//TODO Use thread for solve and show progress
//TODO Add icon and about box

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), m_invalidEntries(0)
{
    ui->setupUi(this);
    connect(ui->gridView, &GridView::lockedCell, this, &MainWindow::onLockedEntry);
    connect(ui->gridView, &GridView::invalidEntry, this, &MainWindow::onInvalidEntry);
    statusLabel = new QLabel();
    statusLabel->setText("Invalid Entries: 0");
    ui->statusbar->addPermanentWidget(statusLabel);
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
    ++m_invalidEntries;
    statusLabel->setText(QString("Invalid Entries: %1").arg(m_invalidEntries));
}


void MainWindow::on_actionExit_triggered()
{
    close();
}


void MainWindow::on_actionLock_Editing_triggered()
{
    ui->gridView->lockGrid(true);
    m_invalidEntries = 0;
    statusLabel->setText("Invalid Entries: 0");
    update();
}


void MainWindow::on_actionUnlock_Editing_triggered()
{
    ui->gridView->lockGrid(false);
    update();
}


void MainWindow::on_actionNew_triggered()
{
    ui->gridView->newGrid();
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


void MainWindow::on_actionOpen_triggered()
{
    QString fileName;
    QSettings settings;
    QString lastFile = settings.value("LastFile", "").toString();
    fileName = QFileDialog::getOpenFileName(this, "Open puzze", lastFile, "Sudoku puzzels (*.sud)");
    if (fileName != "") {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        ui->gridView->load(in);
        file.close();
        settings.setValue("LastFile", fileName);
        update();
    }
}


void MainWindow::on_actionSave_triggered()
{
    QString fileName;
    QSettings settings;
    QString lastFile = settings.value("LastFile", "").toString();
    fileName = QFileDialog::getSaveFileName(this, "Save puzzle", lastFile, "Sudoku puzzels (*.sud)");
    if (fileName != "") {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        ui->gridView->save(out);
        file.close();
        QSettings settings;
        settings.setValue("LastFile", fileName);
    }
}


void MainWindow::on_actionSolve_triggered()
{
    if (ui->gridView->solve()) {
        update();
    } else {
        QMessageBox::critical(this, "Sudoku", "No solution possible");
    }
}


void MainWindow::on_actionShow_Pencil_Marks_toggled(bool arg1)
{
    ui->gridView->showMarks(arg1);
}


void MainWindow::on_actionRefresh_Pencil_Marks_triggered()
{
    ui->gridView->refreshMarks();
}

