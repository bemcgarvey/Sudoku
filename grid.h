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
    const QList<int>& getPossible(int r, int c) const;
private:
    bool isValidEntry(int r, int c, int value);
    void updatePossibles(int r, int c, int newValue, int oldValue);
    QVector<QVector<int>> m_values;
    QVector<QVector<QList<int>>> m_possible;
};

#endif // GRID_H
