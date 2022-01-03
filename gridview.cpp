#include "gridview.h"

#include <QFont>
#include <QMouseEvent>
#include <QPainter>

GridView::GridView(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f), m_grid(nullptr), m_selectedX(-1), m_selectedY(-1)
{

}

void GridView::setGrid(Grid *grid)
{
    m_grid.reset(grid);
}

void GridView::lockGrid(bool lock)
{
    if (lock) {
        m_grid->lock();
    } else {
        m_grid->unlock();
    }
}


void GridView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    int groupSize = m_side / 3;
    int boxSize = groupSize / 3 - 1;
    QFont largeFont = painter.font();
    largeFont.setPixelSize(boxSize - 10);
    QFont smallFont = painter.font();
    smallFont.setPixelSize(boxSize / 5);
    QPen widePen;
    widePen.setWidth(3);
    QPen smallPen;
    smallPen.setWidth(1);
    QBrush colorBrush(Qt::yellow);
    QBrush whiteBrush(Qt::white);
    painter.setBrush(whiteBrush);
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            int x = m_margin + r * groupSize;
            int y = m_margin + c * groupSize;
            painter.setPen(widePen);
            painter.drawRect(x, y, groupSize, groupSize);
            painter.setPen(smallPen);
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (r * 3 + i == m_selectedX && c * 3 + j == m_selectedY) {
                        painter.save();
                        painter.setBrush(colorBrush);
                        painter.drawRect(x + 1 + i * boxSize, y + 1 + j * boxSize, boxSize, boxSize);
                        painter.restore();
                    } else {
                        painter.drawRect(x + 1 + i * boxSize, y + 1 + j * boxSize, boxSize, boxSize);
                    }
                    if (m_grid) {
                        bool fixed;
                        int value;
                        value = m_grid->value(r * 3 + i, c * 3 + j, fixed);
                        if (value != GridCell::EMPTY_CELL) {
                            painter.setFont(largeFont);
                            if (fixed) {
                                painter.setPen(Qt::blue);
                            } else {
                                painter.setPen(Qt::black);
                            }
                            painter.drawText(x + 1 + i * boxSize,
                                             y + 1 + j * boxSize,
                                             boxSize, boxSize,
                                             Qt::AlignHCenter | Qt::AlignVCenter,
                                             QString().setNum(value));
                            painter.setPen(Qt::black);
                        } else {
                            painter.setFont(smallFont);
                            int ty;
                            int tx;
                            const QBitArray &marks = m_grid->getMarkup(r * 3 + i, c * 3 + j);
                            for (int n = 1; n <= 9; ++n) {
                                if (!marks[n]) {
                                    continue;
                                }
                                if (n <= 3) {
                                    tx = x + 2;
                                }
                                if (n >= 4 && n <= 6) {
                                    tx = (x + 2) + (boxSize - boxSize / 5) / 2;
                                } else if (n >= 7) {
                                    tx = (x + 2) + boxSize - boxSize / 5;
                                }
                                ty = 1 + (((n - 1) % 3) * (boxSize / 5));
                                painter.drawText(tx + 1 + i * boxSize,
                                                 y + 1 + j * boxSize + ty,
                                                 boxSize / 5, boxSize / 5,
                                                 Qt::AlignLeft | Qt::AlignVCenter,
                                                 QString().setNum(n));

                            }
                        }
                    }
                }
            }
        }
    }
}

void GridView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    if (width() > height()) {
        m_side = height() - 2 * m_margin;
    } else {
        m_side = width() - 2 * m_margin;
    }
    m_side -= m_side % 9;
}

void GridView::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    m_selectedX = -1;
    m_selectedY = -1;
    if (pos.x() > m_margin && pos.y() > m_margin) {
        m_selectedX = (pos.x() - m_margin) / (m_side / 9);
        m_selectedY = (pos.y() - m_margin) / (m_side / 9);
    }
    if (m_selectedX > 8 || m_selectedY > 8) {
        m_selectedX = -1;
        m_selectedY = -1;
    }
    setFocus();
    update();

}


void GridView::keyPressEvent(QKeyEvent *event)
{
    if (m_selectedX >= 0 && m_selectedY >= 0) {
        if (event->key() >= Qt::Key_1 && event->key() <= Qt::Key_9) {
            if (event->modifiers() & Qt::AltModifier) {
                m_grid->toggleMarkup(m_selectedX, m_selectedY, event->key() - Qt::Key_0);
            } else {
                Grid::GridErrorType result;
                result = m_grid->setValue(m_selectedX, m_selectedY, event->key() - Qt::Key_0);
                if (result == Grid::ERROR_LOCKED_CELL) {
                    emit lockedCell();
                } else if (result == Grid::ERROR_INVALID_ENTRY) {
                    emit invalidEntry();
                }
            }
        } else if (event->key() == Qt::Key_Space) {
            m_grid->setValue(m_selectedX, m_selectedY, 0);
        } else {
            switch (event->key()) {
            case Qt::Key_Left:
                if (m_selectedX > 0) {
                    --m_selectedX;
                }
                break;
            case Qt::Key_Right:
                if (m_selectedX < 8) {
                    ++m_selectedX;
                }
                break;
            case Qt::Key_Up:
                if (m_selectedY > 0) {
                    --m_selectedY;
                }
                break;
            case Qt::Key_Down:
                if (m_selectedY < 8) {
                    ++m_selectedY;
                }
                break;
            }
        }
        update();
    }
}
