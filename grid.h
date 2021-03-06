#ifndef GRID_H
#define GRID_H

#include <QVector>
#include "gridcell.h"

class SolveThread;

class Grid
{
friend SolveThread;
public:
    Grid();
    enum GridErrorType {NO_ERROR, ERROR_LOCKED_CELL, ERROR_INVALID_ENTRY};
    int value(int r, int c, bool &fixed) const;
    GridErrorType setValue(int r, int c, int value);
    const QBitArray getMarkup(int r, int c) const;
    void removeMarkup(int r, int c, int value);
    void addMarkup(int r, int c, int value);
    void toggleMarkup(int r, int c, int value);
    bool solve();
    SolveThread* solveWithThread();
    void lock();
    void unlock();
    void updateCells(bool refresh = false);
    void clear();
    friend QDataStream & operator<< (QDataStream& stream, const Grid& grid);
    friend QDataStream & operator>> (QDataStream& stream, Grid& grid);
private:
    bool m_locked;
    QVector<QVector<GridCell>> m_cells;
    bool solveCell(int r, int c);
};

QDataStream & operator<< (QDataStream& stream, const Grid& grid);
QDataStream & operator>> (QDataStream& stream, Grid& grid);

#endif // GRID_H
