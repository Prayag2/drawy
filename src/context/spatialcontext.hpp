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

#include <QWidget>
class QuadTree;
class CacheGrid;
class CoordinateTransformer;
class ApplicationContext;
class CommandHistory;
class Canvas;

class SpatialContext : public QObject {
public:
    SpatialContext(ApplicationContext *context);
    ~SpatialContext();

    void setSpatialContext();

    // SpatialContext
    QuadTree &quadtree() const;
    CacheGrid &cacheGrid() const;
    CoordinateTransformer &coordinateTransformer() const;
    CommandHistory &commandHistory() const;

    const QPointF &offsetPos() const;
    void setOffsetPos(const QPointF &pos);

    void reset();

private:
    std::unique_ptr<QuadTree> m_quadtree{nullptr};
    std::unique_ptr<CacheGrid> m_cacheGrid{nullptr};
    std::unique_ptr<CoordinateTransformer> m_coordinateTransformer{nullptr};
    std::unique_ptr<CommandHistory> m_commandHistory{nullptr};

    // Stores the position of the topleft corner of the viewport with respect to
    // to the world center. If viewport moves down/right, the coordinates increase
    QPointF m_offsetPos{};

    ApplicationContext *m_applicationContext;
};
