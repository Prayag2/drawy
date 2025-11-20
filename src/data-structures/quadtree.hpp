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

#include <QPainter>
#include <QRectF>
#include <QVector>
#include <memory>
#include <unordered_map>

#include "../item/item.hpp"

class OrderedList;

/*
 * A QuadTree is a tree data structure which divides the space,
 * or in our case, the canvas, into 4 regions recursively.
 * It will be useful here to detect collisions with existing items
 * And redraw only the items necessary which improves performance.
 */
/*
 * NOTE: This is tightly coupled with the OrderedList data structure present in
 * the same directory and the Item class present in the `item` directory.
 */
class QuadTree {
public:
    using ItemPtr = std::shared_ptr<Item>;

private:
    QVector<ItemPtr> m_items{};
    QRectF m_boundingBox{};
    int m_capacity{};
    std::unique_ptr<QuadTree> m_topLeft{nullptr};
    std::unique_ptr<QuadTree> m_topRight{nullptr};
    std::unique_ptr<QuadTree> m_bottomRight{nullptr};
    std::unique_ptr<QuadTree> m_bottomLeft{nullptr};
    std::shared_ptr<OrderedList> m_orderedList{nullptr};

public:
    QuadTree(QRectF region, int capacity);
    QuadTree(QRectF region, int capacity, std::shared_ptr<OrderedList> orderedList);

    ~QuadTree();

    int size() const;
    void insertItem(ItemPtr item, bool updateOrder = true);
    void deleteItem(ItemPtr item, bool updateOrder = true);
    void updateItem(ItemPtr item, const QRectF &oldBoundingBox);
    void deleteItems(const QRectF &boundingBox);

    void reorder(QVector<ItemPtr>& items) const;

    QVector<ItemPtr> getAllItems() const;
    void clear();

    template <typename Shape, typename QueryCondition>
    QVector<ItemPtr> queryItems(const Shape &shape, QueryCondition condition) const;

    template <typename Shape>
    QVector<ItemPtr> queryItems(const Shape &shape) const;

    void draw(QPainter &painter, const QPointF &offset) const;
    const QRectF &boundingBox() const;

private:
    bool insert(ItemPtr item, bool updateOrder);
    void update(ItemPtr item, const QRectF &oldBoundingBox, bool inserted);

    template <typename Shape, typename QueryCondition>
    void query(const Shape &shape,
               QueryCondition condition,
               QVector<ItemPtr> &out,
               std::unordered_map<ItemPtr, bool> &itemAlreadyPushed) const;

    void subdivide();
    void expand(const QPointF &point);
};

#include "quadtree.ipp"
