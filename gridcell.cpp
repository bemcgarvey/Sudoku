#include "gridcell.h"
#include <QDataStream>

GridCell::GridCell()
    : m_value(EMPTY_CELL), m_fixed(false), m_possible(10, true), m_userMarks(10, true)
{

}

QDataStream & operator<< (QDataStream& stream, const GridCell& cell) {
    stream << cell.m_value << cell.m_fixed << cell.m_possible << cell.m_userMarks;
    return stream;
}

QDataStream & operator>> (QDataStream& stream, GridCell& cell) {
    stream >> cell.m_value >> cell.m_fixed >> cell.m_possible >> cell.m_userMarks;
    return stream;
}
