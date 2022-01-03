#include "grid.h"

Grid::Grid()
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
        updateCells();
    } else if (m_cells[r][c].m_possible[value]) {
        m_cells[r][c].m_value = value;
        updateCells();
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
    return false;
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
    //TODO update markup to match possible
}

void Grid::updateCells()
{
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
