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

#include "erasertool.hpp"

#include <QDebug>
#include <QPainter>

#include "../canvas/canvas.hpp"
#include "../command/commandhistory.hpp"
#include "../command/removeitemcommand.hpp"
#include "../common/constants.hpp"
#include "../common/renderitems.hpp"
#include "../context/applicationcontext.hpp"
#include "../context/coordinatetransformer.hpp"
#include "../context/renderingcontext.hpp"
#include "../context/selectioncontext.hpp"
#include "../context/spatialcontext.hpp"
#include "../context/uicontext.hpp"
#include "../data-structures/cachegrid.hpp"
#include "../data-structures/quadtree.hpp"
#include "../event/event.hpp"
#include "../item/item.hpp"
#include "../properties/widgets/propertymanager.hpp"

EraserTool::EraserTool() {
    m_cursor = QCursor(Qt::CrossCursor);

    m_properties = {Property::EraserSize};
}

void EraserTool::mousePressed(ApplicationContext *context) {
    Event &event{context->uiContext().event()};

    if (event.button() == Qt::LeftButton) {
        m_isErasing = true;
    }
};

// FIXME: messy code
void EraserTool::mouseMoved(ApplicationContext *context) {
    SpatialContext &spatialContext{context->spatialContext()};
    RenderingContext &renderingContext{context->renderingContext()};
    UIContext &uiContext{context->uiContext()};
    CoordinateTransformer &transformer{spatialContext.coordinateTransformer()};

    QPainter &overlayPainter{renderingContext.overlayPainter()};

    // Erase previous box
    overlayPainter.save();
    overlayPainter.setCompositionMode(QPainter::CompositionMode_Source);
    overlayPainter.fillRect(m_lastRect + Common::cleanupMargin, Qt::transparent);

    const int eraserSide{uiContext.propertyManager().value(Property::EraserSize).value<int>()};
    const QSize eraserSize{eraserSide, eraserSide};

    // TODO: Adjustable eraser size
    double eraserCenterOffset{eraserSide / 2.0 - 1};
    QPointF eraserCenterOffsetPoint{eraserCenterOffset, eraserCenterOffset};

    QRectF curRect{uiContext.event().pos() - eraserCenterOffsetPoint, eraserSize};
    QRectF worldEraserRect{transformer.viewToWorld(curRect)};

    if (m_isErasing) {
        QVector<std::shared_ptr<Item>> toBeErased{
            spatialContext.quadtree().queryItems(worldEraserRect)};

        for (std::shared_ptr<Item> item : toBeErased) {
            if (m_toBeErased.count(item) > 0)
                continue;

            item->setProperty(Property::Opacity,
                              Property{Common::eraseItemOpacity, Property::Opacity});

            m_toBeErased.insert(item);
            spatialContext.cacheGrid().markDirty(
                transformer.worldToGrid(item->boundingBox()).toRect());
            renderingContext.markForRender();
        }

        overlayPainter.fillRect(curRect, Common::eraserBackgroundColor);
    }

    renderingContext.markForUpdate();

    // Draw eraser box
    QPen pen{Common::eraserBorderColor, Common::eraserBorderWidth};
    overlayPainter.setPen(pen);
    overlayPainter.drawRect(curRect);
    overlayPainter.restore();

    renderingContext.markForUpdate();

    m_lastRect = curRect;
}

void EraserTool::mouseReleased(ApplicationContext *context) {
    UIContext &uiContext{context->uiContext()};

    if (uiContext.event().button() == Qt::LeftButton) {
        SpatialContext &spatialContext{context->spatialContext()};
        CoordinateTransformer &transformer{spatialContext.coordinateTransformer()};
        RenderingContext &renderingContext{context->renderingContext()};
        SelectionContext &selectionContext{context->selectionContext()};
        CommandHistory &commandHistory{spatialContext.commandHistory()};

        QVector<std::shared_ptr<Item>> erasedItems;
        for (std::shared_ptr<Item> item : m_toBeErased) {
            if (selectionContext.selectedItems().count(item) > 0) {
                selectionContext.selectedItems().erase(item);
            }

            // reset opacity
            item->setProperty(Property::Opacity,
                              Property{Common::maxItemOpacity, Property::Opacity});
            erasedItems.push_back(item);
        }

        if (!erasedItems.empty()) {
            commandHistory.insert(std::make_shared<RemoveItemCommand>(erasedItems));
        }

        renderingContext.markForRender();
        renderingContext.markForUpdate();

        m_toBeErased.clear();
        m_isErasing = false;
    }
}

void EraserTool::leave(ApplicationContext *context) {
    cleanup();
};

void EraserTool::cleanup() {
    ApplicationContext *context{ApplicationContext::instance()};

    context->uiContext().event().setButton(Qt::LeftButton);
    mouseReleased(context);

    auto &overlayPainter{context->renderingContext().overlayPainter()};
    overlayPainter.save();

    overlayPainter.setCompositionMode(QPainter::CompositionMode_Source);
    overlayPainter.fillRect(m_lastRect + Common::cleanupMargin, Qt::transparent);

    context->renderingContext().markForUpdate();

    overlayPainter.restore();
}

Tool::Type EraserTool::type() const {
    return Tool::Eraser;
}

QString EraserTool::tooltip() const {
    return "Eraser Tool";
}

IconManager::Icon EraserTool::icon() const {
    return IconManager::TOOL_ERASER;
};
