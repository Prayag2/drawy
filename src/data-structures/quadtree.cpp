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

#include "quadtree.hpp"

#include <QDebug>
#include <cstdlib>
#include <memory>

#include "../item/item.hpp"
#include "orderedlist.hpp"

QuadTree::QuadTree(QRectF region, int capacity) : m_boundingBox{region}, m_capacity{capacity} {
    if (m_orderedList == nullptr) {
        m_orderedList = std::make_shared<OrderedList>();
    }
}

QuadTree::QuadTree(QRectF region, int capacity, std::shared_ptr<OrderedList> orderedList)
    : m_boundingBox{region},
      m_capacity{capacity} {
    m_orderedList = orderedList;
}

QuadTree::~QuadTree() {
    qDebug() << "Object deleted: QuadTree";
}

void QuadTree::subdivide() {
    double x{m_boundingBox.x()};
    double y{m_boundingBox.y()};
    double halfWidth{m_boundingBox.width() / 2};
    double halfHeight{m_boundingBox.height() / 2};

    QRectF topLeftRect{x, y, halfWidth, halfHeight};
    QRectF topRightRect{x + halfWidth, y, halfWidth, halfHeight};
    QRectF bottomRightRect{x + halfWidth, y + halfHeight, halfWidth, halfHeight};
    QRectF bottomLeftRect{x, y + halfHeight, halfWidth, halfHeight};

    m_topLeft = std::make_unique<QuadTree>(topLeftRect, m_capacity, m_orderedList);
    m_topRight = std::make_unique<QuadTree>(topRightRect, m_capacity, m_orderedList);
    m_bottomRight = std::make_unique<QuadTree>(bottomRightRect, m_capacity, m_orderedList);
    m_bottomLeft = std::make_unique<QuadTree>(bottomLeftRect, m_capacity, m_orderedList);
}

void QuadTree::insertItem(std::shared_ptr<Item> item, bool updateOrder) {
    expand(item->boundingBox().topLeft());
    expand(item->boundingBox().topRight());
    expand(item->boundingBox().bottomRight());
    expand(item->boundingBox().bottomLeft());
    insert(item, updateOrder);
}

bool QuadTree::insert(std::shared_ptr<Item> item, bool updateOrder) {
    if (!m_boundingBox.intersects(item->boundingBox())) {
        return false;
    }

    if (m_items.size() < m_capacity) {
        m_items.push_back(item);
        
        if (updateOrder)
            m_orderedList->insert(item);

        return true;
    }

    // subdivide if not already subdivided
    if (m_topLeft == nullptr)
        subdivide();

    bool inserted = false;
    if (m_topLeft->insert(item, updateOrder))
        inserted = true;
    if (m_topRight->insert(item, updateOrder))
        inserted = true;
    if (m_bottomRight->insert(item, updateOrder))
        inserted = true;
    if (m_bottomLeft->insert(item, updateOrder))
        inserted = true;

    return inserted;
}

void QuadTree::deleteItem(std::shared_ptr<Item> const item, bool updateOrder) {
    if (!m_boundingBox.intersects(item->boundingBox())) {
        return;
    }

    auto it = std::find(m_items.begin(), m_items.end(), item);
    if (it != m_items.end()) {
        m_items.erase(it);

        if (updateOrder)
            m_orderedList->remove(item);

        return;
    }

    // If the node is subdivided, attempt to delete the item from children
    if (m_topLeft != nullptr) {
        m_topLeft->deleteItem(item, updateOrder);
        m_topRight->deleteItem(item, updateOrder);
        m_bottomLeft->deleteItem(item, updateOrder);
        m_bottomRight->deleteItem(item, updateOrder);
    }
}

void QuadTree::clear() {
    m_items.clear();

    if (m_topLeft != nullptr) {
        m_topLeft->clear();
        m_topRight->clear();
        m_bottomRight->clear();
        m_bottomLeft->clear();
    }
}

void QuadTree::reorder(QVector<ItemPtr>& items) const {
    std::sort(items.begin(), items.end(), [&](auto &firstItem, auto &secondItem) {
        return m_orderedList->zIndex(firstItem) < m_orderedList->zIndex(secondItem);
    });
}

void QuadTree::updateItem(std::shared_ptr<Item> item, const QRectF &oldBoundingBox) {
    expand(item->boundingBox().topLeft());
    expand(item->boundingBox().topRight());
    expand(item->boundingBox().bottomRight());
    expand(item->boundingBox().bottomLeft());

    update(item, oldBoundingBox, false);
}

void QuadTree::update(std::shared_ptr<Item> item, const QRectF &oldBoundingBox, bool inserted) {
    if (!m_boundingBox.intersects(oldBoundingBox) &&
        !m_boundingBox.intersects(item->boundingBox())) {
        return;
    }

    if (m_boundingBox.intersects(oldBoundingBox)) {
        auto it = std::find(m_items.begin(), m_items.end(), item);
        if (it != m_items.end()) {
            m_items.erase(it);
        }
    }

    if (!inserted && m_boundingBox.intersects(item->boundingBox())) {
        if (m_items.size() < m_capacity) {
            m_items.push_back(item);
            inserted = true;
        } else if (m_topLeft == nullptr)
            subdivide();
    }

    if (m_topLeft != nullptr) {
        m_topLeft->update(item, oldBoundingBox, inserted);
        m_topRight->update(item, oldBoundingBox, inserted);
        m_bottomLeft->update(item, oldBoundingBox, inserted);
        m_bottomRight->update(item, oldBoundingBox, inserted);
    }
}

void QuadTree::deleteItems(const QRectF &boundingBox) {
    if (!m_boundingBox.intersects(boundingBox))
        return;

    for (int i = 0; i < m_items.size();) {
        if (boundingBox.intersects(m_items[i]->boundingBox())) {
            m_items.erase(m_items.begin() + i);
            m_orderedList->remove(m_items[i]);
        } else {
            i++;
        }
    }

    if (m_topLeft != nullptr) {
        m_topLeft->deleteItems(boundingBox);
        m_topRight->deleteItems(boundingBox);
        m_bottomRight->deleteItems(boundingBox);
        m_bottomLeft->deleteItems(boundingBox);
    }
}

QVector<std::shared_ptr<Item>> QuadTree::getAllItems() const {
    QVector<std::shared_ptr<Item>> curItems{m_items};
    if (m_topLeft != nullptr) {
        curItems += m_topLeft->getAllItems();
        curItems += m_topRight->getAllItems();
        curItems += m_bottomRight->getAllItems();
        curItems += m_bottomLeft->getAllItems();
    }
    return curItems;
}

const QRectF &QuadTree::boundingBox() const {
    return m_boundingBox;
};

int QuadTree::size() const {
    int totalNodes{static_cast<int>(m_items.size())};

    if (m_topLeft != nullptr) {
        totalNodes += m_topLeft->size();
        totalNodes += m_topRight->size();
        totalNodes += m_bottomRight->size();
        totalNodes += m_bottomLeft->size();
    }
    return totalNodes;
}

void QuadTree::draw(QPainter &painter, const QPointF &offset) const {
    painter.save();

    QPen pen{Qt::green};
    painter.setPen(pen);
    painter.drawRect(m_boundingBox.translated(-offset));
    painter.restore();

    if (m_topLeft != nullptr) {
        m_topLeft->draw(painter, offset);
        m_topRight->draw(painter, offset);
        m_bottomRight->draw(painter, offset);
        m_bottomLeft->draw(painter, offset);
    }
}

void QuadTree::expand(const QPointF &point) {
    // This function grows the quadtree in size recursively if the
    // point lies outside of it, making it almost infinite!
    if (m_boundingBox.contains(point))
        return;

    double treeW{m_boundingBox.width()}, treeH{m_boundingBox.height()};
    double x{point.x()}, y{point.y()};
    QPointF topLeftPoint{m_boundingBox.topLeft()}, bottomRightPoint{m_boundingBox.bottomRight()};

    std::unique_ptr<QuadTree> topLeft{
        std::make_unique<QuadTree>(m_boundingBox, m_capacity, m_orderedList)};
    std::unique_ptr<QuadTree> topRight{
        std::make_unique<QuadTree>(m_boundingBox, m_capacity, m_orderedList)};
    std::unique_ptr<QuadTree> bottomRight{
        std::make_unique<QuadTree>(m_boundingBox, m_capacity, m_orderedList)};
    std::unique_ptr<QuadTree> bottomLeft{
        std::make_unique<QuadTree>(m_boundingBox, m_capacity, m_orderedList)};
    std::unique_ptr<QuadTree> cur{};

    if (x < topLeftPoint.x() || y < topLeftPoint.y()) {
        m_boundingBox.adjust(-treeW, -treeH, 0, 0);

        topLeft->m_boundingBox.translate(-treeW, -treeH);
        topRight->m_boundingBox.translate(0, -treeH);
        bottomLeft->m_boundingBox.translate(-treeW, 0);

        cur = std::move(bottomRight);
    } else if (x > bottomRightPoint.x() || y > bottomRightPoint.y()) {
        m_boundingBox.adjust(0, 0, treeW, treeH);

        topRight->m_boundingBox.translate(treeW, 0);
        bottomRight->m_boundingBox.translate(treeW, treeH);
        bottomLeft->m_boundingBox.translate(0, treeH);

        cur = std::move(topLeft);
    }

    if (m_topLeft) {
        cur->m_topLeft = std::move(m_topLeft);
        cur->m_topRight = std::move(m_topRight);
        cur->m_bottomRight = std::move(m_bottomRight);
        cur->m_bottomLeft = std::move(m_bottomLeft);
    }
    cur->m_items = std::move(m_items);

    if (topLeft == nullptr)
        topLeft = std::move(cur);
    else if (bottomRight == nullptr)
        bottomRight = std::move(cur);

    m_topLeft = std::move(topLeft);
    m_topRight = std::move(topRight);
    m_bottomRight = std::move(bottomRight);
    m_bottomLeft = std::move(bottomLeft);

    // expand recursively
    expand(point);
}
