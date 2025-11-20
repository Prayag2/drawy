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

#include "selectiontoolselectstate.hpp"
#include "../../command/selectcommand.hpp"
#include "../../command/deselectcommand.hpp"
#include "../../command/commandhistory.hpp"
#include "../../canvas/canvas.hpp"
#include "../../components/propertybar.hpp"
#include "../../context/applicationcontext.hpp"
#include "../../context/coordinatetransformer.hpp"
#include "../../context/renderingcontext.hpp"
#include "../../context/selectioncontext.hpp"
#include "../../context/spatialcontext.hpp"
#include "../../context/uicontext.hpp"
#include "../../data-structures/cachegrid.hpp"
#include "../../data-structures/quadtree.hpp"
#include "../../event/event.hpp"
#include "../../item/item.hpp"

bool SelectionToolSelectState::mousePressed(ApplicationContext *context) {
    auto &uiContext{context->uiContext()};
    auto &event{uiContext.event()};

    if (event.button() == Qt::LeftButton) {
        m_lastPos = uiContext.event().pos();

        auto &spatialContext{context->spatialContext()};
        auto &selectionContext{context->selectionContext()};
        auto &renderingContext{context->renderingContext()};
        auto &transformer{spatialContext.coordinateTransformer()};

        QVector<std::shared_ptr<Item>> intersectingItems{
            spatialContext.quadtree().queryItems(transformer.viewToWorld(m_lastPos),
                                                 [](std::shared_ptr<Item> item, auto &pos) {
                                                     return item->boundingBox().contains(pos);
                                                 })};

        bool lockState = true;
        auto &selectedItems{selectionContext.selectedItems()};
        auto &commandHistory{spatialContext.commandHistory()};

        if (!(event.modifiers() & Qt::ShiftModifier)) {
            QVector<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
            commandHistory.insert(std::make_shared<DeselectCommand>(items));
        }

        if (intersectingItems.empty()) {
            m_isActive = true;
        } else {
            auto& item{intersectingItems.back()};
            if ((event.modifiers() & Qt::ShiftModifier) && selectedItems.find(item) != selectedItems.end()) {
                // deselect the item if selected
                commandHistory.insert(std::make_shared<DeselectCommand>(QVector<std::shared_ptr<Item>>{item}));
            } else {
                commandHistory.insert(std::make_shared<SelectCommand>(QVector<std::shared_ptr<Item>>{item}));
            }
            m_isActive = false;
            lockState = false;
        }

        context->uiContext().propertyBar().updateToolProperties();
        renderingContext.markForRender();
        renderingContext.markForUpdate();

        return lockState;
    }

    return true;
}

void SelectionToolSelectState::mouseMoved(ApplicationContext *context) {
    auto &renderingContext{context->renderingContext()};
    auto &spatialContext{context->spatialContext()};
    renderingContext.canvas().setCursor(Qt::ArrowCursor);

    auto &painter{renderingContext.overlayPainter()};
    if (!m_isActive) {
        return;
    }

    auto &uiContext{context->uiContext()};
    auto &transformer{spatialContext.coordinateTransformer()};
    auto &selectionContext{context->selectionContext()};
    auto &selectedItems{selectionContext.selectedItems()};

    renderingContext.canvas().overlay()->fill(Qt::transparent);

    QPointF curPos{uiContext.event().pos()};

    QRectF selectionBox{m_lastPos, curPos};
    QRectF worldSelectionBox{transformer.viewToWorld(selectionBox)};

    QVector<std::shared_ptr<Item>> intersectingItems{
        spatialContext.quadtree().queryItems(worldSelectionBox,
                                             [](std::shared_ptr<Item> item, const QRectF &rect) {
                                                 return rect.contains(item->boundingBox());
                                             })};

    selectedItems = std::unordered_set(intersectingItems.begin(), intersectingItems.end());
    context->uiContext().propertyBar().updateToolProperties();

    QPainter &overlayPainter{renderingContext.overlayPainter()};
    overlayPainter.save();

    // TODO: Remove magic numbers
    QPen pen{QColor{67, 135, 244, 200}};
    overlayPainter.setPen(pen);

    overlayPainter.drawRect(selectionBox);
    overlayPainter.fillRect(selectionBox, QColor{67, 135, 244, 50});

    overlayPainter.restore();

    renderingContext.markForRender();
    renderingContext.markForUpdate();
}

bool SelectionToolSelectState::mouseReleased(ApplicationContext *context) {
    if (m_isActive) {
        auto &renderingContext{context->renderingContext()};
        auto &selectedItems{context->selectionContext().selectedItems()};
        auto &commandHistory{context->spatialContext().commandHistory()};

        if (!selectedItems.empty()) {
            QVector<std::shared_ptr<Item>> items{};
            for (const auto item : selectedItems) {
                items.push_back(item);
            }

            selectedItems.clear();
            commandHistory.insert(std::make_shared<SelectCommand>(items));
        }

        renderingContext.canvas().overlay()->fill(Qt::transparent);
        renderingContext.markForUpdate();

        m_isActive = false;
    }

    return false;
}
