#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <QWidget>
#include "grid.h"

class GridView : public QWidget
{
public:
    GridView(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    void setGrid(Grid *grid);
private:
    std::unique_ptr<Grid> m_grid;
    const int m_margin = 5;
    int m_side;
    int m_selectedX;
    int m_selectedY;
protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // GRIDVIEW_H
