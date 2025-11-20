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

#include "deselectcommand.hpp"

#include "../context/applicationcontext.hpp"
#include "../context/coordinatetransformer.hpp"
#include "../context/selectioncontext.hpp"
#include "../context/spatialcontext.hpp"
#include "../item/item.hpp"
#include "../data-structures/cachegrid.hpp"

DeselectCommand::DeselectCommand(QVector<std::shared_ptr<Item>> items) : ItemCommand{items} {}

DeselectCommand::~DeselectCommand() {
}

void DeselectCommand::execute(ApplicationContext *context) {
    auto &selectedItems{context->selectionContext().selectedItems()};

    QRectF dirtyRegion{};
    for (const auto item : m_items) {
        dirtyRegion |= item->boundingBox();
        selectedItems.erase(item);
    }

    context->spatialContext().cacheGrid().markDirty(dirtyRegion.toRect());
}

void DeselectCommand::undo(ApplicationContext *context) {
    auto &selectedItems{context->selectionContext().selectedItems()};

    QRectF dirtyRegion{};
    for (const auto item : m_items) {
        dirtyRegion |= item->boundingBox();
        selectedItems.insert(item);
    }

    context->spatialContext().cacheGrid().markDirty(dirtyRegion.toRect());
}
