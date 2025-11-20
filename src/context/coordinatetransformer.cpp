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

#include "coordinatetransformer.hpp"

#include <cmath>

#include "applicationcontext.hpp"
#include "renderingcontext.hpp"
#include "spatialcontext.hpp"

CoordinateTransformer::CoordinateTransformer(ApplicationContext *context)
    : m_applicationContext{context} {};

CoordinateTransformer::~CoordinateTransformer() {
    qDebug() << "Object deleted: CoordinateTransformer";
}

void CoordinateTransformer::setCoordinateTransformer() {
    m_spatialContext = &(m_applicationContext->spatialContext());
    m_renderingContext = &(m_applicationContext->renderingContext());
}

QPointF CoordinateTransformer::worldToView(QPointF point) {
    return (point - m_spatialContext->offsetPos()) * m_renderingContext->zoomFactor();
}

QSizeF CoordinateTransformer::worldToView(QSizeF size) {
    return size * m_renderingContext->zoomFactor();
}

QRectF CoordinateTransformer::worldToView(QRectF rect) {
    return {worldToView(rect.topLeft()), worldToView(rect.size())};
}

QPointF CoordinateTransformer::viewToWorld(QPointF point) {
    return point / m_renderingContext->zoomFactor() + m_spatialContext->offsetPos();
}

QSizeF CoordinateTransformer::viewToWorld(QSizeF size) {
    return size / m_renderingContext->zoomFactor();
}

QRectF CoordinateTransformer::viewToWorld(QRectF rect) {
    return {viewToWorld(rect.topLeft()), viewToWorld(rect.size())};
}

QPoint CoordinateTransformer::worldToView(QPoint point) {
    return round(worldToView(point.toPointF()));
}

QSize CoordinateTransformer::worldToView(QSize size) {
    return round(worldToView(size.toSizeF()));
}

QRect CoordinateTransformer::worldToView(QRect rect) {
    return round(worldToView(rect.toRectF()));
}

QPoint CoordinateTransformer::viewToWorld(QPoint point) {
    return round(viewToWorld(point.toPointF()));
}

QSize CoordinateTransformer::viewToWorld(QSize size) {
    return round(viewToWorld(size.toSizeF()));
}

QRect CoordinateTransformer::viewToWorld(QRect rect) {
    return round(viewToWorld(rect.toRectF()));
}

QPointF CoordinateTransformer::worldToGrid(QPointF point) {
    return point * m_renderingContext->zoomFactor();
}

QSizeF CoordinateTransformer::worldToGrid(QSizeF size) {
    return size * m_renderingContext->zoomFactor();
}

QRectF CoordinateTransformer::worldToGrid(QRectF rect) {
    return {worldToGrid(rect.topLeft()), worldToGrid(rect.size())};
}

QPointF CoordinateTransformer::gridToWorld(QPointF point) {
    return point / m_renderingContext->zoomFactor();
}

QSizeF CoordinateTransformer::gridToWorld(QSizeF size) {
    return size / m_renderingContext->zoomFactor();
}

QRectF CoordinateTransformer::gridToWorld(QRectF rect) {
    return {gridToWorld(rect.topLeft()), gridToWorld(rect.size())};
}

QPoint CoordinateTransformer::worldToGrid(QPoint point) {
    return round(worldToGrid(point.toPointF()));
}

QSize CoordinateTransformer::worldToGrid(QSize size) {
    return round(worldToGrid(size.toSizeF()));
}

QRect CoordinateTransformer::worldToGrid(QRect rect) {
    return round(worldToGrid(rect.toRectF()));
}

QPoint CoordinateTransformer::gridToWorld(QPoint point) {
    return round(gridToWorld(point.toPointF()));
}

QSize CoordinateTransformer::gridToWorld(QSize size) {
    return round(gridToWorld(size.toSizeF()));
}

QRect CoordinateTransformer::gridToWorld(QRect rect) {
    return round(gridToWorld(rect.toRectF()));
}

QPointF CoordinateTransformer::viewToGrid(QPointF point) {
    return point + worldToView(m_spatialContext->offsetPos());
}

QSizeF CoordinateTransformer::viewToGrid(QSizeF size) {
    return size;
}

QRectF CoordinateTransformer::viewToGrid(QRectF rect) {
    return QRectF{viewToGrid(rect.topLeft()), viewToGrid(rect.bottomRight())};
}

QPointF CoordinateTransformer::gridToView(QPointF point) {
    return point - worldToGrid(m_spatialContext->offsetPos());
}

QSizeF CoordinateTransformer::gridToView(QSizeF size) {
    return size;
}

QRectF CoordinateTransformer::gridToView(QRectF rect) {
    return QRectF{gridToView(rect.topLeft()), gridToView(rect.bottomRight())};
}

QPoint CoordinateTransformer::viewToGrid(QPoint point) {
    return round(viewToGrid(point.toPointF()));
}

QSize CoordinateTransformer::viewToGrid(QSize size) {
    return round(viewToGrid(size.toSizeF()));
}

QRect CoordinateTransformer::viewToGrid(QRect rect) {
    return round(viewToGrid(rect.toRectF()));
}

QPoint CoordinateTransformer::gridToView(QPoint point) {
    return round(gridToView(point.toPointF()));
}

QSize CoordinateTransformer::gridToView(QSize size) {
    return round(gridToView(size.toSizeF()));
}

QRect CoordinateTransformer::gridToView(QRect rect) {
    return round(gridToView(rect.toRectF()));
}

QPoint CoordinateTransformer::round(QPointF point) {
    return QPoint{qRound(point.x()), qRound(point.y())};
}

QRect CoordinateTransformer::round(QRectF rect) {
    int x1 = qRound(rect.left());
    int y1 = qRound(rect.top());
    int x2 = qRound(rect.right());
    int y2 = qRound(rect.bottom());

    return QRect{x1, y1, x2 - x1, y2 - y1};
}

QSize CoordinateTransformer::round(QSizeF size) {
    return QSize{qRound(size.width()), qRound(size.height())};
}
