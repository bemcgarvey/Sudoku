#include "grid.h"

Grid::Grid()
{
    QList<int> p = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < 9; ++i) {
        m_values.append(QVector<int>(9, 0));
        m_possible.append(QVector<QList<int>>(9, p));
    }
}

int Grid::value(int r, int c)
{
    return m_values[r][c];
}

bool Grid::setValue(int r, int c, int value)
{
    if (isValidEntry(r, c, value)) {
        int oldValue = m_values[r][c];
        m_values[r][c] = value;
        updatePossibles(r, c, value, oldValue);
        return true;
    } else {
        return false;
    }
}

const QList<int> &Grid::getPossible(int r, int c) const
{
    return m_possible[r][c];
}

bool Grid::isValidEntry(int r, int c, int value)
{
    if (value == 0) {
        return true;
    }
    return m_possible[r][c].contains(value);
}

void Grid::updatePossibles(int r, int c, int newValue, int oldValue)
{
    //TODO handle a changed value
    if (newValue != 0) {
        m_possible[r][c].clear();
        for (int i = 0; i < 9; ++i) {
            //check row
            m_possible[r][i].removeAll(newValue);
            //check column
            m_possible[i][c].removeAll(newValue);
        }
        //Check box
        int boxRow = r - r % 3;
        int boxCol = c - c % 3;
        for (int i = boxRow; i < boxRow + 3; ++i) {
            for (int j = boxCol; j < boxCol + 3; ++j) {
                m_possible[i][j].removeAll(newValue);
            }
        }
    } else {
        //TODO cell is now blank so add possible values back in
    }
}
