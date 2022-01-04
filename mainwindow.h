#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLockedEntry();
    void onInvalidEntry();
    void on_actionExit_triggered();
    void on_actionLock_Editing_triggered();

    void on_actionUnlock_Editing_triggered();

    void on_actionNew_triggered();

    void on_actionPrint_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSolve_triggered();

private:
    Ui::MainWindow *ui;
    int m_invalidAttempts;
};
#endif // MAINWINDOW_H
