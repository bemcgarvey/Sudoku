#ifndef SOLVETHREAD_H
#define SOLVETHREAD_H

#include <QThread>

class Grid;

class SolveThread : public QThread
{
    Q_OBJECT
public:
    explicit SolveThread(Grid *grid, QObject *parent = nullptr);
    void resume(void);
private:
    Grid *m_grid;
    volatile bool m_paused;
    bool solve(int r, int c);
public:
    virtual void run() override;
signals:
    void progress(int row);
};

#endif // SOLVETHREAD_H
