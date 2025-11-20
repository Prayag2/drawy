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

#include "ungroupcommand.hpp"

#include "../context/applicationcontext.hpp"
#include "../context/coordinatetransformer.hpp"
#include "../context/selectioncontext.hpp"
#include "../context/spatialcontext.hpp"
#include "../data-structures/cachegrid.hpp"
#include "../data-structures/quadtree.hpp"
#include "../item/group.hpp"
#include <memory>

UngroupCommand::UngroupCommand(QVector<std::shared_ptr<Item>> items) : ItemCommand{items} {
    for (const auto item : items) {
        if (item->type() == Item::Group) {
            m_groups.push_back(std::dynamic_pointer_cast<GroupItem>(item));
        }
    }
}

UngroupCommand::~UngroupCommand() {
}

void UngroupCommand::execute(ApplicationContext *context) {
    auto &quadtree{context->spatialContext().quadtree()};
    auto &selectedItems{context->selectionContext().selectedItems()};

    selectedItems.clear();

    int count = 0;

    QRectF dirtyRegion{};
    for (const auto group : m_groups) {
        quadtree.deleteItem(group);

        dirtyRegion |= group->boundingBox();

        auto subItems{group->unGroup()};
        for (const auto subItem : subItems) {
            quadtree.insertItem(subItem, false);
            selectedItems.insert(subItem);
        }
    }

    context->spatialContext().cacheGrid().markDirty(dirtyRegion.toRect());
}

void UngroupCommand::undo(ApplicationContext *context) {
    auto &quadtree{context->spatialContext().quadtree()};
    auto &selectedItems{context->selectionContext().selectedItems()};

    selectedItems.clear();

    QRectF dirtyRegion{};
    for (const auto group : m_groups) {
        quadtree.insertItem(group);
        selectedItems.insert(group);
        dirtyRegion |= group->boundingBox();

        auto subItems{group->unGroup()};
        for (const auto subItem : subItems) {
            quadtree.deleteItem(subItem, false);
        }
    }

    context->spatialContext().cacheGrid().markDirty(dirtyRegion.toRect());
}
