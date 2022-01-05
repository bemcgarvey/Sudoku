#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <QPrinter>
#include <QWidget>
#include "grid.h"
#include "solvethread.h"

class GridView : public QWidget
{
    Q_OBJECT
public:
    GridView(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    void newGrid();
    void lockGrid(bool lock);
    void print(QPrinter *printer);
    void save(QDataStream &out);
    void load(QDataStream &in);
    bool solve();
    void solveWithThread();
    void showMarks(bool show);
    void refreshMarks();
    void resumeSolve();
    void clear();
private:
    std::unique_ptr<Grid> m_grid;
    std::unique_ptr<SolveThread> m_solveThread;
    const int m_margin = 5;
    int m_side;
    int m_selectedX;
    int m_selectedY;
    void paint(QPainter &painter, bool useColor = true);
    bool m_showMarks;
    bool m_solving;
private slots:
    void onSolveProgress(int r);
protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
signals:
    void lockedCell();
    void invalidEntry();
};

#endif // GRIDVIEW_H
