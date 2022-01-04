#include "solvethread.h"
#include "grid.h"
#include "gridcell.h"

SolveThread::SolveThread(Grid *grid, QObject *parent) :
    QThread(parent), m_grid(grid), m_paused(false)
{

}

void SolveThread::resume()
{
    m_paused = false;
}

bool SolveThread::solve(int r, int c)
{
    if (r == 9) {
        emit progress(r);
        return true; //All cells are filled
    }
    if (c == 0 && r != 0) {
        emit progress(r);
        while (m_paused);
    }
    int nextCol;
    int nextRow = r;
    nextCol = c + 1;
    if  (nextCol == 9) {
        nextCol = 0;
        ++nextRow;
    }
    if (m_grid->m_cells[r][c].m_fixed) {
        return solve(nextRow, nextCol);  //nothing to do here so move on
    }
    for (int i = 1; i <= 9; ++i) {
        if (m_grid->m_cells[r][c].m_possible[i]) {
            m_grid->setValue(r, c, i);
            if (solve(nextRow, nextCol)) {
                return true;
            }
        }
    }
    m_grid->setValue(r, c, GridCell::EMPTY_CELL); //back out last value
    return false;
}


void SolveThread::run()
{
    solve(0, 0);
}
