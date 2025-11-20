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

#pragma once
#include <QHash>
#include <QPixmap>
#include <QPoint>

class CacheGrid;

// Based on LRU cache
class CacheCell {
public:
    static QSize cellSize();
    static int counter;

    CacheCell(const QPoint &point);
    ~CacheCell();

    QRect rect() const;
    const QPoint &point() const;
    bool dirty() const;
    void setDirty(bool dirty);
    QPixmap &image() const;
    QPainter &painter() const;

private:
    QPoint m_point{};
    std::unique_ptr<QPixmap> m_image{nullptr};
    std::unique_ptr<QPainter> m_painter{};
    std::weak_ptr<CacheCell> nextCell{};
    std::weak_ptr<CacheCell> prevCell{};
    bool m_dirty{};

    // CacheGrid can access private members
    friend CacheGrid;
};

class CacheGrid {
public:
    CacheGrid(int maxSize);
    ~CacheGrid();

    QVector<std::shared_ptr<CacheCell>> queryCells(const QRect &rect);
    std::shared_ptr<CacheCell> cell(const QPoint &point);
    void markDirty(const QRect &rect);
    void markAllDirty();
    void setSize(int newSize);
    int size() const;

private:
    QHash<QPoint, std::shared_ptr<CacheCell>> m_grid{};
    std::shared_ptr<CacheCell> m_headCell{std::make_shared<CacheCell>(QPoint{0, 0})};
    std::shared_ptr<CacheCell> m_tailCell{std::make_shared<CacheCell>(QPoint{0, 0})};

    QSize m_cellSize{};
    int m_curSize{0};
    int m_maxSize{0};
};
