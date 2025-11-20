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

#ifndef GROUP_H
#define GROUP_H

#include "item.hpp"

class GroupItem : public Item {
public:
    GroupItem() = default;

    void draw(QPainter &painter, const QPointF &offset) override;
    void erase(QPainter &painter, const QPointF &offset) const override;

    bool intersects(const QRectF &rect) override;
    bool intersects(const QLineF &rect) override;

    void translate(const QPointF &amount) override;

    void group(const QVector<std::shared_ptr<Item>>& items);
    QVector<std::shared_ptr<Item>> unGroup();

    void setProperty(const Property::Type propertyType, Property newObj) override;
    const Property property(const Property::Type propertyType) const override;
    const QVector<Property> properties() const override;
    const QVector<Property::Type> propertyTypes() const override;

    const QRectF boundingBox() const override;

    Item::Type type() const override;

private:
    QVector<std::shared_ptr<Item>> m_items;

    void m_draw(QPainter &painter, const QPointF &offset) const override;
};

#endif  // GROUP_H
