#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setApplicationName("Sudoku");
    QApplication::setOrganizationName("QES");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
