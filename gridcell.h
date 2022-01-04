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
    friend QDataStream & operator<< (QDataStream& stream, const GridCell& cell);
    friend QDataStream & operator>> (QDataStream& stream, GridCell& cell);
private:
    int m_value;
    bool m_fixed;
    QBitArray m_possible;
    QBitArray m_userMarks;
};

QDataStream & operator<< (QDataStream& stream, const GridCell& cell);
QDataStream & operator>> (QDataStream& stream, GridCell& cell);

#endif // GRIDCELL_H
