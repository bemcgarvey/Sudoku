#include "grid.h"

Grid::Grid()
{
    QVector<bool> p(9, true);
    for (int i = 0; i < 9; ++i) {
        m_values.append(QVector<int>(9, 0));
        m_possible.append(QVector<QVector<bool>>(9, p));
    }
}

int Grid::value(int r, int c)
{
    return m_values[r][c];
}

bool Grid::setValue(int r, int c, int value)
{
    if (isValidEntry(r, c, value)) {
        m_values[r][c] = value;
        updatePossibles();
        return true;
    } else {
        return false;
    }
}

const QVector<bool> &Grid::getPossible(int r, int c) const
{
    return m_possible[r][c];
}

bool Grid::isValidEntry(int r, int c, int value)
{
    if (value == 0) {
        return true;
    }
    return m_possible[r][c][value - 1];
}

void Grid::updatePossibles()
{
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            for (int v = 1; v <= 9; ++v) {
                m_possible[r][c][v - 1] = isPossible(r, c, v);
            }
        }
    }
}

bool Grid::isPossible(int r, int c, int value)
{
    for (int i = 0; i < 9; ++i) {
        if (i != c && m_values[r][i] == value) {
            return false;
        }
        if (i != r && m_values[i][c] == value) {
            return false;
        }
    }
    int boxRow = r - r % 3;
    int boxCol = c - c % 3;
    for (int i = boxRow; i < boxRow + 3; ++i) {
        for (int j = boxCol; j < boxCol + 3; ++j) {
            if (i != r && j != c && m_values[i][j] == value) {
                return false;
            }
        }
    }
    return true;
}
