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

#include "selectioncontext.hpp"

#include "../command/commandhistory.hpp"
#include "../command/updatepropertycommand.hpp"
#include "../data-structures/cachegrid.hpp"
#include "../item/item.hpp"
#include "../properties/property.hpp"
#include "applicationcontext.hpp"
#include "coordinatetransformer.hpp"
#include "renderingcontext.hpp"
#include "spatialcontext.hpp"

SelectionContext::SelectionContext(ApplicationContext *context)
    : QObject{context},
      m_applicationContext{context} {
}

SelectionContext::~SelectionContext() {
    qDebug() << "Object deleted: SelectionContext";
}

std::unordered_set<std::shared_ptr<Item>> &SelectionContext::selectedItems() {
    return m_selectedItems;
}

QRectF SelectionContext::selectionBox() const {
    QRectF selectionBox;
    for (auto item : m_selectedItems) {
        selectionBox |= item->boundingBox();
    }
    return selectionBox;
}

// PUBLIC SLOTS
void SelectionContext::updatePropertyOfSelectedItems(Property property) {
    QVector<std::shared_ptr<Item>> items{m_selectedItems.begin(), m_selectedItems.end()};

    auto &commandHistory{m_applicationContext->spatialContext().commandHistory()};
    commandHistory.insert(std::make_shared<UpdatePropertyCommand>(items, property));

    m_applicationContext->renderingContext().markForRender();
    m_applicationContext->renderingContext().markForUpdate();
}

void SelectionContext::reset() {
    selectedItems().clear();
}
