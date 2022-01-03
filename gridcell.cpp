#include "gridcell.h"

GridCell::GridCell()
    : m_value(EMPTY_CELL), m_fixed(false), m_possible(10, true), m_userMarks(10, true)
{

}
