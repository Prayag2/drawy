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

#include "../common/utils/math.hpp"
#include "../item/item.hpp"
#include "orderedlist.hpp"
#include <cstdlib>
#include <memory>
#include <unordered_map>

template <typename Shape>
QVector<std::shared_ptr<Item>> QuadTree::queryItems(const Shape &shape) const {
    return queryItems(shape, [](std::shared_ptr<Item> item, const Shape &shape) {
        return item->intersects(shape);
    });
}

template <typename Shape, typename QueryCondition>
QVector<std::shared_ptr<Item>> QuadTree::queryItems(const Shape &shape,
                                                    QueryCondition condition) const {
    QVector<std::shared_ptr<Item>> curItems{};
    std::unordered_map<std::shared_ptr<Item>, bool> itemAlreadyPushed{};

    // look for matches and store the result in curItems
    query(shape, condition, curItems, itemAlreadyPushed);

    // sort based on z-index
    std::sort(curItems.begin(), curItems.end(), [&](auto &firstItem, auto &secondItem) {
        return m_orderedList->zIndex(firstItem) < m_orderedList->zIndex(secondItem);
    });

    return curItems;
};

template <typename Shape, typename QueryCondition>
void QuadTree::query(const Shape &shape,
                     QueryCondition condition,
                     QVector<std::shared_ptr<Item>> &out,
                     std::unordered_map<std::shared_ptr<Item>, bool> &itemAlreadyPushed) const {
    if (!Common::Utils::Math::intersects(m_boundingBox, shape)) {
        return;
    }

    for (const std::shared_ptr<Item> &item : m_items) {
        if (Common::Utils::Math::intersects(item->boundingBox(), shape)) {
            if (condition(item, shape)) {
                // using the hash map because multiple nodes may have a pointer to the
                // same item
                if (!itemAlreadyPushed[item]) {
                    out.push_back(item);
                    itemAlreadyPushed[item] = true;
                }
            }
        }
    }

    // if this node has sub-regions
    if (m_topLeft != nullptr) {
        m_topLeft->query(shape, condition, out, itemAlreadyPushed);
        m_topRight->query(shape, condition, out, itemAlreadyPushed);
        m_bottomRight->query(shape, condition, out, itemAlreadyPushed);
        m_bottomLeft->query(shape, condition, out, itemAlreadyPushed);
    }
}
