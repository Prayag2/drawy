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

#include "groupcommand.hpp"

#include "commandhistory.hpp"
#include "selectcommand.hpp"
#include "deselectcommand.hpp"
#include "../context/applicationcontext.hpp"
#include "../context/coordinatetransformer.hpp"
#include "../context/selectioncontext.hpp"
#include "../context/spatialcontext.hpp"
#include "../data-structures/cachegrid.hpp"
#include "../data-structures/quadtree.hpp"
#include "../item/group.hpp"

GroupCommand::GroupCommand(QVector<std::shared_ptr<Item>> items) : ItemCommand{items} {
    m_group = std::make_shared<GroupItem>();

    // sort according to z order
    ApplicationContext::instance()->spatialContext().quadtree().reorder(m_items);
}

GroupCommand::~GroupCommand() {
}

void GroupCommand::execute(ApplicationContext *context) {
    auto &quadtree{context->spatialContext().quadtree()};
    auto &selectedItems{context->selectionContext().selectedItems()};

    for (const auto item : m_items) {
        quadtree.deleteItem(item, false);
    }

    m_group->group(m_items);
    quadtree.insertItem(m_group);

    selectedItems.clear();
    selectedItems.insert(m_group);

    context->spatialContext().cacheGrid().markDirty(m_group->boundingBox().toRect());
}

void GroupCommand::undo(ApplicationContext *context) {
    auto &quadtree{context->spatialContext().quadtree()};
    auto &selectedItems{context->selectionContext().selectedItems()};

    quadtree.deleteItem(m_group);
    selectedItems.clear();

    for (const auto item : m_items) {
        selectedItems.insert(item);
        quadtree.insertItem(item, false);
    }

    context->spatialContext().cacheGrid().markDirty(m_group->boundingBox().toRect());
}
