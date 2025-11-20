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

#include <list>
#include <memory>
#include <unordered_map>
class Item;

// Keeps track of the z-index of every item
class OrderedList {
public:
    using ItemPtr = std::shared_ptr<Item>;

private:
    std::unordered_map<ItemPtr, std::list<ItemPtr>::iterator> m_itemIterMap;
    std::list<ItemPtr> m_itemList;
    std::unordered_map<ItemPtr, int> m_zIndex;

public:
    ~OrderedList();

    void insert(ItemPtr item);
    void remove(ItemPtr item);

    void bringForward(ItemPtr item);
    void sendBackward(ItemPtr item);
    void sendToBack(ItemPtr item);
    void bringToFront(ItemPtr item);
    bool hasItem(ItemPtr item) const;

    int zIndex(ItemPtr item) const;
};
