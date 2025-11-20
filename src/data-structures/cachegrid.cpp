/*
 * Drawy - A simple brainstorming tool with an infinite canvas
 * Copyright (C) 2025 - Prayag Jain <prayagjain2@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "cachegrid.hpp"

#include <QDebug>
#include <QPainter>

int CacheCell::counter = 0;

CacheCell::CacheCell(const QPoint &point) : m_point{point} {
    m_image = std::make_unique<QPixmap>(CacheCell::cellSize());
    m_image->fill(Qt::transparent);

    m_painter = std::make_unique<QPainter>(m_image.get());
    m_painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    m_painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    m_painter->setClipRegion(m_image->rect());

    CacheCell::counter++;
    m_dirty = true;
}

CacheCell::~CacheCell() {
    CacheCell::counter--;
}

const QPoint &CacheCell::point() const {
    return m_point;
}

bool CacheCell::dirty() const {
    return m_dirty;
}

QPixmap &CacheCell::image() const {
    return *m_image;
}

QRect CacheCell::rect() const {
    int cellW{CacheCell::cellSize().width()}, cellH{CacheCell::cellSize().height()};
    QPoint cellPos{point().x() * cellW, point().y() * cellH};
    return {cellPos.x(), cellPos.y(), cellW, cellH};
}

void CacheCell::setDirty(bool dirty) {
    m_dirty = dirty;
}

QPainter &CacheCell::painter() const {
    return *m_painter;
}

QSize CacheCell::cellSize() {
    return {500, 500};
}

CacheGrid::CacheGrid(int maxSize) {
    m_headCell->nextCell = m_tailCell;
    m_tailCell->prevCell = m_headCell;

    if (maxSize == 0) {
        throw std::logic_error("maxSize can not be zero for the cache grid");
    }
    m_maxSize = maxSize;
}

CacheGrid::~CacheGrid() {
    qDebug() << "Object deleted: CacheGrid";
}

QVector<std::shared_ptr<CacheCell>> CacheGrid::queryCells(const QRect &rect) {
    QPoint topLeft{rect.topLeft()}, bottomRight{rect.bottomRight()};

    int cellMinX = floor(static_cast<double>(topLeft.x()) / CacheCell::cellSize().width());
    int cellMinY = floor(static_cast<double>(topLeft.y()) / CacheCell::cellSize().height());
    int cellMaxX = floor(static_cast<double>(bottomRight.x()) / CacheCell::cellSize().width());
    int cellMaxY = floor(static_cast<double>(bottomRight.y()) / CacheCell::cellSize().height());

    QVector<std::shared_ptr<CacheCell>> out{};
    for (int row = cellMinX; row <= cellMaxX; row++) {
        for (int col = cellMinY; col <= cellMaxY; col++) {
            out.push_back(cell(QPoint{row, col}));
        }
    }

    return out;
}

void CacheGrid::markDirty(const QRect &rect) {
    QVector<std::shared_ptr<CacheCell>> dirtyCells{queryCells(rect)};
    for (std::shared_ptr<CacheCell> cell : dirtyCells) {
        cell->setDirty(true);
    }
}

std::shared_ptr<CacheCell> CacheGrid::cell(const QPoint &point) {
    std::shared_ptr<CacheCell> cur{};
    if (!m_grid.contains(point) || !m_grid[point]) {
        if (m_curSize == m_maxSize) {
            // deleting least recently used cell
            std::shared_ptr<CacheCell> temp{m_headCell->nextCell};
            m_headCell->nextCell = temp->nextCell;
            if (auto next = temp->nextCell.lock()) {
                next->prevCell = m_headCell;
            }
            m_grid[temp->point()] = nullptr;
            m_grid.remove(temp->point());
            temp.reset();
            m_curSize--;
        }

        cur = std::make_shared<CacheCell>(point);
        m_grid[point] = cur;
        m_curSize++;
    } else {
        cur = m_grid[point];
        if (auto prev = cur->prevCell.lock()) {
            prev->nextCell = cur->nextCell;
        }
        if (auto next = cur->nextCell.lock()) {
            next->prevCell = cur->prevCell;
        }
    }
    cur->prevCell = m_tailCell->prevCell;
    cur->nextCell = m_tailCell;
    if (auto prev = m_tailCell->prevCell.lock()) {
        prev->nextCell = cur;
    }
    m_tailCell->prevCell = cur;

    return cur;
}

int CacheGrid::size() const {
    return m_curSize;
}

void CacheGrid::setSize(int newSize) {
    m_maxSize = newSize;
}

void CacheGrid::markAllDirty() {
    for (auto cell : m_grid) {
        cell->setDirty(true);
    }
}
