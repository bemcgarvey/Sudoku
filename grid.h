#ifndef GRID_H
#define GRID_H

#include <QVector>
#include <QList>

class Grid
{
public:
    Grid();
    int value(int r, int c);
    bool setValue(int r, int c, int value);
    const QVector<bool>& getPossible(int r, int c) const;
    void removePossible(int r, int c, int value);
private:
    bool isValidEntry(int r, int c, int value);
    void updatePossibles(int r, int c);
    bool isPossible(int r, int c, int value);
    QVector<QVector<int>> m_values;
    QVector<QVector<QVector<bool>>> m_possible;
};

#endif // GRID_H
