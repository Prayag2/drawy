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

#include <QRect>
class ApplicationContext;
class SpatialContext;
class RenderingContext;

/*
 * There are three coordinate systems in drawly:
 *  1. World (the one used by the QuadTree to store items)
 *  2. Grid (the one used by the CacheGrid to cache tiles)
 *  3. View (the viewport) */
class CoordinateTransformer {
private:
    SpatialContext *m_spatialContext;
    RenderingContext *m_renderingContext;
    ApplicationContext *m_applicationContext;

public:
    CoordinateTransformer(ApplicationContext *context);
    ~CoordinateTransformer();

    void setCoordinateTransformer();

    QPointF worldToView(QPointF point);
    QSizeF worldToView(QSizeF size);
    QRectF worldToView(QRectF rect);
    QPointF viewToWorld(QPointF point);
    QSizeF viewToWorld(QSizeF size);
    QRectF viewToWorld(QRectF rect);

    QPoint worldToView(QPoint point);
    QSize worldToView(QSize size);
    QRect worldToView(QRect rect);
    QPoint viewToWorld(QPoint point);
    QSize viewToWorld(QSize size);
    QRect viewToWorld(QRect rect);

    QPointF worldToGrid(QPointF point);
    QSizeF worldToGrid(QSizeF size);
    QRectF worldToGrid(QRectF rect);
    QPointF gridToWorld(QPointF point);
    QSizeF gridToWorld(QSizeF size);
    QRectF gridToWorld(QRectF rect);

    QPoint worldToGrid(QPoint point);
    QSize worldToGrid(QSize size);
    QRect worldToGrid(QRect rect);
    QPoint gridToWorld(QPoint point);
    QSize gridToWorld(QSize size);
    QRect gridToWorld(QRect rect);

    QPointF viewToGrid(QPointF point);
    QSizeF viewToGrid(QSizeF size);
    QRectF viewToGrid(QRectF rect);
    QPointF gridToView(QPointF point);
    QSizeF gridToView(QSizeF size);
    QRectF gridToView(QRectF rect);

    QPoint viewToGrid(QPoint point);
    QSize viewToGrid(QSize size);
    QRect viewToGrid(QRect rect);
    QPoint gridToView(QPoint point);
    QSize gridToView(QSize size);
    QRect gridToView(QRect rect);

    QPoint round(QPointF point);
    QRect round(QRectF rect);
    QSize round(QSizeF size);
};
