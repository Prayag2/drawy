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

#include "orderedlist.hpp"

#include <QDebug>
#include <stdexcept>

OrderedList::~OrderedList() {
    qDebug() << "Object deleted: OrderedList";
}

bool OrderedList::hasItem(ItemPtr item) const {
    if (m_itemIterMap.find(item) == m_itemIterMap.end()) {
        return false;
    }
    if (m_zIndex.find(item) == m_zIndex.end()) {
        return false;
    }
    return true;
}

void OrderedList::insert(ItemPtr item) {
    // item already exists
    if (hasItem(item)) {
        return;
    }

    int zIndex{0};
    if (!m_itemList.empty()) {
        zIndex = m_zIndex[m_itemList.back()] + 1;
    }

    qDebug() << "Inserting item with index: " << zIndex;
    m_zIndex[item] = zIndex;
    m_itemList.push_back(item);
    m_itemIterMap[item] = std::prev(m_itemList.end());
}

void OrderedList::remove(ItemPtr item) {
    // item already deleted
    if (!hasItem(item)) {
        return;
    }

    qDebug() << "Erasing item from list";
    m_itemList.erase(m_itemIterMap[item]);
    m_itemIterMap.erase(item);
    m_zIndex.erase(item);
}

void OrderedList::bringForward(ItemPtr item) {
    if (!hasItem(item)) {
        throw std::runtime_error("Item was not found in the iterator map");
    }

    auto iterator = m_itemIterMap[item];

    // if this is the last element, no need to bring it to the front
    if (iterator == std::prev(m_itemList.end())) {
        return;
    }

    // swap with next
    auto nextIterator = std::next(iterator);
    m_itemList.splice(iterator, m_itemList, nextIterator);

    int val = m_zIndex[item];
    m_zIndex[item] = m_zIndex[*nextIterator];
    m_zIndex[*nextIterator] = val;
}

void OrderedList::sendBackward(ItemPtr item) {
    if (!hasItem(item)) {
        throw std::runtime_error("Item was not found in the iterator map");
    }

    auto iterator = m_itemIterMap[item];

    // if this is the first element, no need to send it to the back
    if (iterator == m_itemList.begin()) {
        return;
    }

    // swap with previous
    auto prevIterator = std::prev(iterator);
    m_itemList.splice(prevIterator, m_itemList, iterator);

    int val = m_zIndex[item];
    m_zIndex[item] = m_zIndex[*prevIterator];
    m_zIndex[*prevIterator] = val;
};

void OrderedList::sendToBack(ItemPtr item) {
    if (!hasItem(item)) {
        throw std::runtime_error("Item was not found in the iterator map");
    }

    auto iterator = m_itemIterMap[item];
    if (iterator == m_itemList.begin()) {
        return;
    }

    auto firstIterator = m_itemList.begin();
    m_itemList.splice(firstIterator, m_itemList, iterator);
    m_zIndex[item] = m_zIndex[*firstIterator] - 1;
}

void OrderedList::bringToFront(ItemPtr item) {
    if (!hasItem(item)) {
        throw std::runtime_error("Item was not found in the iterator map");
    }

    auto iterator = m_itemIterMap[item];
    if (iterator == std::prev(m_itemList.end())) {
        return;
    }

    auto lastIterator = std::prev(m_itemList.end());
    m_itemList.splice(m_itemList.end(), m_itemList, iterator);
    m_zIndex[item] = m_zIndex[*lastIterator] + 1;
}

int OrderedList::zIndex(ItemPtr item) const {
    if (!hasItem(item)) {
        throw std::runtime_error("Item not found in zIndex map");
    }
    return m_zIndex.at(item);
}
