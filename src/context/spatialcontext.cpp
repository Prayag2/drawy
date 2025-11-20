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

#include "spatialcontext.hpp"

#include <memory>

#include "../canvas/canvas.hpp"
#include "../command/commandhistory.hpp"
#include "../data-structures/cachegrid.hpp"
#include "../data-structures/quadtree.hpp"
#include "applicationcontext.hpp"
#include "coordinatetransformer.hpp"
#include "renderingcontext.hpp"

SpatialContext::SpatialContext(ApplicationContext *context)
    : QObject{context},
      m_applicationContext{context} {
}

SpatialContext::~SpatialContext() {
    qDebug() << "Object deleted: SpatialContext";
}

void SpatialContext::setSpatialContext() {
    Canvas &canvas{m_applicationContext->renderingContext().canvas()};

    m_quadtree = std::make_unique<QuadTree>(QRect{{0, 0}, canvas.sizeHint()}, 100);
    m_coordinateTransformer = std::make_unique<CoordinateTransformer>(m_applicationContext);
    m_cacheGrid = std::make_unique<CacheGrid>(100);
    m_commandHistory = std::make_unique<CommandHistory>(m_applicationContext);
}

QuadTree &SpatialContext::quadtree() const {
    return *m_quadtree;
}

CacheGrid &SpatialContext::cacheGrid() const {
    return *m_cacheGrid;
}

CoordinateTransformer &SpatialContext::coordinateTransformer() const {
    return *m_coordinateTransformer;
}

CommandHistory &SpatialContext::commandHistory() const {
    return *m_commandHistory;
}

const QPointF &SpatialContext::offsetPos() const {
    return m_offsetPos;
}

void SpatialContext::setOffsetPos(const QPointF &pos) {
    m_offsetPos = pos;
}

void SpatialContext::reset() {
    quadtree().clear();
    cacheGrid().markAllDirty();
    commandHistory().clear();
    setOffsetPos(QPointF{0, 0});
}
