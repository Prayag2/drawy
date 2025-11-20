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

#include "renderitems.hpp"

#include <QPointF>
#include <QRectF>
#include <memory>

#include "../canvas/canvas.hpp"
#include "../context/applicationcontext.hpp"
#include "../context/coordinatetransformer.hpp"
#include "../context/renderingcontext.hpp"
#include "../context/selectioncontext.hpp"
#include "../context/spatialcontext.hpp"
#include "../data-structures/cachegrid.hpp"
#include "../data-structures/quadtree.hpp"
#include "../item/item.hpp"
#include "constants.hpp"

// TODO: Refactor this
void Common::renderCanvas(ApplicationContext *context) {
    CoordinateTransformer &transformer{context->spatialContext().coordinateTransformer()};
    Canvas &canvas{context->renderingContext().canvas()};
    QPointF offsetPos{context->spatialContext().offsetPos()};

    canvas.canvas()->fill(canvas.bg());

    QPointF gridOffset{transformer.worldToGrid(offsetPos)};
    QRectF gridViewport(gridOffset, transformer.viewToGrid(canvas.dimensions()));

    QVector<std::shared_ptr<CacheCell>> visibleCells{
        context->spatialContext().cacheGrid().queryCells(transformer.round(gridViewport))};

    QPainter &canvasPainter{context->renderingContext().canvasPainter()};

    for (auto cell : visibleCells) {
        // canvasPainter.save();
        // QPen pen; pen.setColor(Qt::white); canvasPainter.setPen(pen);
        // canvasPainter.drawRect(transformer.gridToView(cell->rect()));
        // canvasPainter.restore();

        if (cell->dirty()) {
            cell->image().fill(Qt::transparent);
            cell->setDirty(false);

            QVector<std::shared_ptr<Item>> intersectingItems{
                context->spatialContext().quadtree().queryItems(
                    transformer.gridToWorld(cell->rect()),
                    [](auto a, auto b) { return true; })};

            if (intersectingItems.empty())
                continue;

            qreal zoomFactor{context->renderingContext().zoomFactor()};

            QPointF topLeftPoint{transformer.gridToWorld(cell->rect().topLeft().toPointF())};

            cell->painter().resetTransform();
            cell->painter().scale(zoomFactor, zoomFactor);

            for (auto intersectingItem : intersectingItems) {
                intersectingItem->draw(cell->painter(), topLeftPoint);
            }
        }

        canvasPainter.drawPixmap(transformer.round(transformer.gridToView(cell->rect())),
                                 cell->image());
    }

    QRectF selectionBox{};
    auto &selectedItems{context->selectionContext().selectedItems()};

    if (selectedItems.empty())
        return;

    // render a box around selected items
    canvasPainter.save();
    QPen pen{Common::selectionBorderColor};
    pen.setWidth(2);

    canvasPainter.setPen(pen);

    for (auto item : selectedItems) {
        QRectF curBox{transformer.worldToView(item->boundingBox()).normalized()};
        canvasPainter.drawRect(curBox);
        selectionBox |= curBox;
    }

    canvasPainter.setPen(pen);
    canvasPainter.drawRect(selectionBox);
    canvasPainter.restore();
}
