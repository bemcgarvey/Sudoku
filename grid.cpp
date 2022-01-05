#include "grid.h"
#include <QDataStream>
#include "solvethread.h"

Grid::Grid() : m_locked(false)
{
    for (int i = 0; i < 9; ++i) {
        m_cells.append(QVector<GridCell>(9));
    }
}

int Grid::value(int r, int c, bool &fixed) const
{
    fixed = m_cells[r][c].m_fixed;
    return m_cells[r][c].m_value;
}

Grid::GridErrorType Grid::setValue(int r, int c, int value)
{
    if (m_cells[r][c].m_fixed) {
        return ERROR_LOCKED_CELL;
    }
    if (value == GridCell::EMPTY_CELL) {
        m_cells[r][c].m_value = value;
        updateCells(true);
    } else if (m_cells[r][c].m_possible[value]) {
        bool refresh = false;
        if (m_cells[r][c].m_value != GridCell::EMPTY_CELL && m_cells[r][c].m_value != value) {
            refresh = true;
        }
        m_cells[r][c].m_value = value;
        updateCells(refresh);
    } else {
        return ERROR_INVALID_ENTRY;
    }
    return NO_ERROR;
}

const QBitArray Grid::getMarkup(int r, int c) const
{
    return m_cells[r][c].m_userMarks;
}

void Grid::removeMarkup(int r, int c, int value)
{
    m_cells[r][c].m_userMarks[value] = false;
}

void Grid::addMarkup(int r, int c, int value)
{
    m_cells[r][c].m_userMarks[value] = true;
}

void Grid::toggleMarkup(int r, int c, int value)
{
    m_cells[r][c].m_userMarks[value] = !m_cells[r][c].m_userMarks[value];
}

bool Grid::solve()
{
    return solveCell(0, 0);
}

SolveThread *Grid::solveWithThread()
{
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (m_cells[r][c].m_value != GridCell::EMPTY_CELL) {
                m_cells[r][c].m_fixed = true;
            }
        }
    }
    updateCells(true);
    SolveThread *sThread = new SolveThread(this, nullptr);
    return sThread;
}

void Grid::lock()
{
    if (m_locked) {
        return; //Already locked
    }
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (m_cells[r][c].m_value != GridCell::EMPTY_CELL) {
                m_cells[r][c].m_fixed = true;
            }
        }
    }
    m_locked = true;
}

void Grid::unlock()
{
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            m_cells[r][c].m_fixed = false;
        }
    }
    m_locked = false;
    updateCells(true);
}

void Grid::updateCells(bool refresh)
{
    if (refresh || !m_locked) {
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                m_cells[r][c].m_possible.fill(true);
                m_cells[r][c].m_userMarks.fill(true);
            }
        }
    }
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (m_cells[r][c].m_value != GridCell::EMPTY_CELL) {
                for (int i = 0; i < 9; ++i) {
                    m_cells[r][i].m_possible[m_cells[r][c].m_value] = false;
                    m_cells[i][c].m_possible[m_cells[r][c].m_value] = false;
                    m_cells[r][i].m_userMarks[m_cells[r][c].m_value] = false;
                    m_cells[i][c].m_userMarks[m_cells[r][c].m_value] = false;
                }
                int boxRow = r - r % 3;
                int boxCol = c - c % 3;
                for (int i = boxRow; i < boxRow + 3; ++i) {
                    for (int j = boxCol; j < boxCol + 3; ++j) {
                        m_cells[i][j].m_possible[m_cells[r][c].m_value] = false;
                        m_cells[i][j].m_userMarks[m_cells[r][c].m_value] = false;
                    }
                }
            }
        }
    }
}

void Grid::clear()
{
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (!m_cells[r][c].m_fixed) {
                m_cells[r][c].m_value = GridCell::EMPTY_CELL;
            }
        }
    }
    updateCells(true);
}

bool Grid::solveCell(int r, int c)
{
    if (r == 9) {
        return true; //All cells are filled
    }
    int nextCol;
    int nextRow = r;
    nextCol = c + 1;
    if  (nextCol == 9) {
        nextCol = 0;
        ++nextRow;
    }
    if (m_cells[r][c].m_fixed) {
        return solveCell(nextRow, nextCol);  //nothing to do here so move on
    }
    for (int i = 1; i <= 9; ++i) {
        if (m_cells[r][c].m_possible[i]) {
            setValue(r, c, i);
            if (solveCell(nextRow, nextCol)) {
                return true;
            }
        }
    }
    setValue(r, c, GridCell::EMPTY_CELL); //back out last value
    return false;
}


QDataStream & operator<< (QDataStream& stream, const Grid& grid) {
    stream << grid.m_locked << grid.m_cells;
    return stream;
}

QDataStream & operator>> (QDataStream& stream, Grid& grid) {
    stream >> grid.m_locked;
    stream >> grid.m_cells;
    return stream;
}
