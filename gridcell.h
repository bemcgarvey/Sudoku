#ifndef GRIDCELL_H
#define GRIDCELL_H

#include <QBitArray>
class Grid;

class GridCell
{
friend Grid;
public:
    GridCell();
    enum {EMPTY_CELL = 0};
private:
    int m_value;
    bool m_fixed;
    QBitArray m_possible;
    QBitArray m_userMarks;
};

#endif // GRIDCELL_H
